// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANSYNCOBJECTS_HPP
#define VKRT_VULKANSYNCOBJECTS_HPP

#include <stdexcept>

#include <vulkan/vulkan.h>

#include "vkrt/core/VulkanDevice.hpp"

namespace vkrt {


/**
 * @class VulkanFence
 * @brief Encapsulates a VkFence object.
 * 
 * Provides a RAII wrapper for a VkFence. Flags can be set through the constructor.
 */
class VulkanFence {
public:
    VulkanFence(const VulkanDevice& vulkanDevice);
    VulkanFence(const VulkanDevice& vulkanDevice, VkFenceCreateFlagBits flags);
    ~VulkanFence();

    VulkanFence(const VulkanFence&) = delete;
    VulkanFence& operator=(const VulkanFence&) = delete;

    VkFence getFence() const { return fence; }

private:
    const VulkanDevice& vulkanDevice;

    VkFence fence;

    void initFence(VkFenceCreateFlagBits flags);
};

/**
 * @class VulkanSemaphore
 * @brief Encapsulates a VkSemaphore object.
 * 
 * Provides a RAII wrapper for a VkSemaphore.
 */
class VulkanSemaphore {
public:
    VulkanSemaphore(const VulkanDevice& vulkanDevice);
    ~VulkanSemaphore();

    VulkanSemaphore(const VulkanSemaphore&) = delete;
    VulkanSemaphore& operator=(const VulkanSemaphore&) = delete;

    VkSemaphore getSemaphore() const { return semaphore; }

private:
    const VulkanDevice& vulkanDevice;

    VkSemaphore semaphore;

    void initSemaphore();
};

} // namespace vkrt

#endif // VKRT_VULKANSYNCOBJECTS_HPP