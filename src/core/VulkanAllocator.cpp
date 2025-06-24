// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/core/VulkanAllocator.hpp"

namespace vkrt {

VulkanAllocator::VulkanAllocator(const VulkanInstance& vulkanInstance, const VulkanDevice& vulkanDevice, const VulkanCommandPool& singleTimePool)
: vulkanInstance(vulkanInstance)
, vulkanDevice(vulkanDevice)
, singleTimePool(singleTimePool)
{
    initAllocator();
}

VulkanAllocator::~VulkanAllocator() {
    vmaDestroyAllocator(allocator);
}

AllocatedBuffer VulkanAllocator::allocateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    VkBufferCreateInfo bufCreateInfo{};
    bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufCreateInfo.size = size;
    bufCreateInfo.usage = usage;

    VmaAllocationCreateInfo allocCreateInfo{};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

    VkBuffer buf;
    VmaAllocation alloc;
    VmaAllocationInfo allocInfo;
    if(vmaCreateBuffer(allocator, &bufCreateInfo, &allocCreateInfo, &buf, &alloc, &allocInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create VMA Buffer!");
    }

    return AllocatedBuffer(buf, size, alloc, allocator);
}

MeshBuffer VulkanAllocator::uploadMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) const {
    size_t vertexBufferSize = sizeof(Vertex) * vertices.size();
    size_t indexBufferSize = sizeof(uint32_t) * indices.size();

    size_t indexBufferOffset = (vertexBufferSize + (4 - 1)) & ~(4 - 1); // Align to 4 bytes because of uint32_t indices

    VkDeviceSize totalSize = indexBufferOffset + indexBufferSize;

    VkBufferCreateInfo stagingBufferInfo{};
    stagingBufferInfo.sType = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    stagingBufferInfo.size = totalSize;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo allocCreateInfo {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | 
                            VMA_ALLOCATION_CREATE_MAPPED_BIT;
    
    VkBuffer stagingBuf;
    VmaAllocation stagingAlloc;
    VmaAllocationInfo stagingInfo;
    vmaCreateBuffer(allocator, &stagingBufferInfo, &allocCreateInfo, &stagingBuf, &stagingAlloc, &stagingInfo);

    memcpy(stagingInfo.pMappedData, vertices.data(), vertexBufferSize);
    std::byte* indexStartAddress = static_cast<std::byte*>(stagingInfo.pMappedData) + indexBufferOffset;
    memcpy(static_cast<void*>(indexStartAddress), indices.data(), indexBufferSize);

    VkBufferCreateInfo localBufferInfo{};
    localBufferInfo.sType = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    localBufferInfo.size = totalSize;
    localBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                            VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
                            VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

    VmaAllocationCreateInfo localAllocCreateInfo {};
    localAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    VkBuffer localBuf;
    VmaAllocation localAlloc;
    vmaCreateBuffer(allocator, &localBufferInfo, &localAllocCreateInfo, &localBuf, &localAlloc, nullptr);

    copyBuffer(stagingBuf, localBuf, totalSize);

    vmaDestroyBuffer(allocator, stagingBuf, stagingAlloc); // Clean up staging buffer

    return MeshBuffer(AllocatedBuffer(localBuf, totalSize, localAlloc, allocator), vertices.size(), indexBufferOffset, indices.size());
}

void VulkanAllocator::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) const {
    VulkanCommandBuffer bufs = singleTimePool.createCommandBuffer();
    VkCommandBuffer buf = bufs.getCommandBuffer();

    VulkanFence fence(vulkanDevice);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(buf, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(buf, src, dst, 1, &copyRegion);

    vkEndCommandBuffer(buf);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buf;

    vkQueueSubmit(vulkanDevice.getPresentQueue(), 1, &submitInfo, fence.getFence());
    vkWaitForFences(vulkanDevice.getDevice(), 1, &fence.getFence(), VK_TRUE, UINT64_MAX);
}

void VulkanAllocator::initAllocator() {
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorInfo{};
    allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorInfo.instance = vulkanInstance.getInstance();
    allocatorInfo.physicalDevice = vulkanDevice.getPhysicalDevice();
    allocatorInfo.device = vulkanDevice.getDevice();
    allocatorInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorInfo, &allocator);
}

AllocatedBuffer::AllocatedBuffer(VkBuffer buffer, VkDeviceSize size, VmaAllocation allocation, VmaAllocator allocator)
: buffer(buffer)
, size(size)
, allocation(allocation)
, allocator(allocator) { }

AllocatedBuffer::~AllocatedBuffer() {
    vmaDestroyBuffer(allocator, buffer, allocation);
}

} // namespace vkrt