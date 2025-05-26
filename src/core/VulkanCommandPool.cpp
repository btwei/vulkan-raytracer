// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/core/VulkanCommandObjects.hpp"

namespace vkrt {

VulkanCommandPool::VulkanCommandPool(const VulkanDevice& vulkanDevice,
                                     VkCommandPoolCreateFlags flags,
                                     uint32_t queueFamilyIndex)
: vulkanDevice(vulkanDevice)
, flags(flags)
, queueFamilyIndex(queueFamilyIndex) {
    createCommandPool();
}

VulkanCommandPool::~VulkanCommandPool() {
    vkDestroyCommandPool(vulkanDevice.getDevice(), commandPool, nullptr);
}

VulkanCommandBuffer VulkanCommandPool::createCommandBuffer() const {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1; 

    std::vector<VkCommandBuffer> commandBuffer(1);
    if(vkAllocateCommandBuffers(vulkanDevice.getDevice(), &allocInfo, commandBuffer.data()) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate command buffer!");
    }

    return VulkanCommandBuffer(vulkanDevice, *this, commandBuffer);
}

VulkanCommandBuffer VulkanCommandPool::createCommandBuffers(uint32_t count) const {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = count; 

    std::vector<VkCommandBuffer> commandBuffers(count);
    if(vkAllocateCommandBuffers(vulkanDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("Failed to allocate command buffer!");
    }

    return VulkanCommandBuffer(vulkanDevice, *this, commandBuffers);
}

void VulkanCommandPool::createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = flags;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if(vkCreateCommandPool(vulkanDevice.getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}

} // namespace vkrt