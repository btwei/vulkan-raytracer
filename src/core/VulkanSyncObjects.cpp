// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/core/VulkanSyncObjects.hpp"

namespace vkrt {

VulkanFence::VulkanFence(const VulkanDevice& vulkanDevice) 
: vulkanDevice(vulkanDevice) { 
    initFence(static_cast<VkFenceCreateFlagBits>(0));
}

VulkanFence::VulkanFence(const VulkanDevice& vulkanDevice, VkFenceCreateFlagBits flags) 
: vulkanDevice(vulkanDevice) {
    initFence(flags);
}

VulkanFence::~VulkanFence() {
    vkDestroyFence(vulkanDevice.getDevice(), fence, nullptr);
}

void VulkanFence::initFence(VkFenceCreateFlagBits flags) {
    VkFenceCreateInfo createInfo = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    createInfo.flags = flags;

    if(vkCreateFence(vulkanDevice.getDevice(), &createInfo, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create fence!");
    }
}

VulkanSemaphore::VulkanSemaphore(const VulkanDevice& vulkanDevice) 
: vulkanDevice(vulkanDevice) {
    initSemaphore();
}

VulkanSemaphore::~VulkanSemaphore() {
    vkDestroySemaphore(vulkanDevice.getDevice(), semaphore, nullptr);
}

void VulkanSemaphore::initSemaphore() {
    VkSemaphoreCreateInfo createInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    if(vkCreateSemaphore(vulkanDevice.getDevice(), &createInfo, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphore!");
    } 
}

} // namespace vkrt