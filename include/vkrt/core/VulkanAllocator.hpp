// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANALLOCATOR_HPP
#define VKRT_VULKANALLOCATOR_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

#include <volk.h>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

#include "vkrt/core/Vertex.hpp"
#include "vkrt/core/VulkanInstance.hpp"
#include "vkrt/core/VulkanDevice.hpp"
#include "vkrt/core/VulkanCommandObjects.hpp"
#include "vkrt/core/VulkanSyncObjects.hpp"

namespace vkrt {

class VulkanAllocator;

/**
 * @class AllocatedBuffer
 * @brief RAII wrapper for a VkBuffer and VmaAllocation
 * 
 * Automatically destroys the buffer memory when out of scope. Can only
 * be constructed by a VulkanAllocator object.
 */
class AllocatedBuffer {
public:
    ~AllocatedBuffer();

    AllocatedBuffer(const AllocatedBuffer&) = delete;
    AllocatedBuffer& operator=(const AllocatedBuffer&) = delete;

    AllocatedBuffer(AllocatedBuffer&& other) noexcept
    : buffer(other.buffer)
    , size(other.size)
    , allocation(other.allocation)
    , allocator(other.allocator) {
        other.buffer = VK_NULL_HANDLE;
        other.size = 0;
        other.allocation = VK_NULL_HANDLE;
    }

    const VkBuffer& getBuffer() const { return buffer; }
    VkDeviceSize getSize() const { return size; }

private:
    AllocatedBuffer(VkBuffer buffer, VkDeviceSize size, VmaAllocation allocation, VmaAllocator allocator);

    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceSize size;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VmaAllocator allocator = VK_NULL_HANDLE;

    friend class VulkanAllocator;
};

struct MeshBuffer {
    AllocatedBuffer buffer;
    uint32_t vertexCount;
    size_t indexOffset;
    uint32_t indexCount;
private:
    MeshBuffer(AllocatedBuffer&& buf, uint32_t vertexCount, size_t indexOffset, uint32_t indexCount)
    : buffer(std::move(buf))
    , vertexCount(vertexCount)
    , indexOffset(indexOffset)
    , indexCount(indexCount) { }

    friend VulkanAllocator;
};

/**
 * @class VulkanAllocator
 * @brief Encapsulates memory allocation for buffers and images.
 * 
 * Provides functions for allocating memory such as for mesh allocations, 
 * blas, tlas, or shader buffer tables. When allocating memory, returns 
 * structs that contain AllocatedBuffer objects.
 */
class VulkanAllocator {
public:
    VulkanAllocator(const VulkanInstance& vulkanInstance, const VulkanDevice& vulkanDevice, const VulkanCommandPool& singleTimePool);
    ~VulkanAllocator();

    VulkanAllocator(const VulkanAllocator&) = delete;
    VulkanAllocator& operator=(const VulkanAllocator&) = delete;

    /**
     * Allocates a VkBuffer wrapped as an AllocatedBuffer object.
     * 
     * @param size The device size of the buffer
     * @param usage The usage flags for the buffer.
     * @param properties The memory property flags for the buffer.
     * @return A newly created AllocatedBuffer object.
     * 
     * @throws std::runtime_error If the buffer allocation fails.
     */
    AllocatedBuffer allocateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) const;
    MeshBuffer uploadMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) const;

private:
    const VulkanInstance& vulkanInstance;
    const VulkanDevice& vulkanDevice;
    const VulkanCommandPool& singleTimePool;

    VmaAllocator allocator;

    void initAllocator();
    void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size) const;
};

} // namespace vkrt

#endif // VKRT_VULKANALLOCATOR_HPP