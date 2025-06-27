// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANCOMMANDOBJECTS_HPP
#define VKRT_VULKANCOMMANDOBJECTS_HPP

#include <stdexcept>
#include <vector>

#include <volk.h>

#include "vkrt/core/VulkanDevice.hpp"

namespace vkrt {

class VulkanCommandBuffer;

/**
 * @class VulkanCommandPool
 * @brief Handles creation, access, and use of a VkCommandPool.
 * 
 * Encapsulates Vulkan command pools. Requires flags and queueFamilyIndex to be specified.
 * Example usages could be creating a pool for each queueFamilyIndex or even creating a
 * dedicated pool for single use command buffers.
 */
class VulkanCommandPool {
public:
    VulkanCommandPool(const VulkanDevice& vulkanDevice, VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
    ~VulkanCommandPool();

    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;

    VkCommandPool getCommandPool() const { return commandPool; }

    VulkanCommandBuffer createCommandBuffer() const;
    VulkanCommandBuffer createCommandBuffers(uint32_t count) const;

private:
    const VulkanDevice& vulkanDevice;
    const VkCommandPoolCreateFlags flags;
    const uint32_t queueFamilyIndex;

    VkCommandPool commandPool;

    void createCommandPool();
};

/**
 * @class VulkanCommandBuffer
 * @brief Encapsulates one or more VkCommandBuffers.
 * 
 * Encapsulates one or more VkCommandBuffers. This object cannot be constructed. To get an
 * instance of this object call createCommandBuffer() on a VulkanCommandPool. VkCommandBuffers
 * are automatically freed when this object is destroyed.
 */
class VulkanCommandBuffer {
public:
    ~VulkanCommandBuffer();

    VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
    VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;

    VkCommandBuffer getCommandBuffer() const { return commandBuffers[0]; }
    VkCommandBuffer getCommandBuffer(size_t index) const { return commandBuffers[index]; };
    std::vector<VkCommandBuffer> getCommandBuffers() const { return commandBuffers; }
    size_t getSize() const { return commandBuffers.size(); }

private:
    const VulkanDevice& vulkanDevice;
    const VulkanCommandPool& vulkanCommandPool;

    std::vector<VkCommandBuffer> commandBuffers;

    VulkanCommandBuffer(const VulkanDevice& vulkanDevice, const VulkanCommandPool& vulkanCommandPool, std::vector<VkCommandBuffer> commandBuffers);

    friend class VulkanCommandPool;
};

} // namespace vkrt

#endif // VKRT_VULKANCOMMANDOBJECTS_HPP