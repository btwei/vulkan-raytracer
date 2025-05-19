// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/core/VulkanCommandObjects.hpp"

namespace vkrt {

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanDevice& vulkanDevice,
                                         const VulkanCommandPool& vulkanCommandPool,
                                         std::vector<VkCommandBuffer> commandBuffers)
: vulkanDevice(vulkanDevice)
, vulkanCommandPool(vulkanCommandPool)
, commandBuffers(commandBuffers) { }

VulkanCommandBuffer::~VulkanCommandBuffer() {
    vkFreeCommandBuffers(vulkanDevice.getDevice(), vulkanCommandPool.getCommandPool(), commandBuffers.size(), commandBuffers.data());
}

} // namespace vkrt