// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/core/VulkanSurface.hpp"

namespace vkrt {

VulkanSurface::VulkanSurface(const Window& window, const VulkanInstance& vulkanInstance) :
    window(window),
    vulkanInstance(vulkanInstance) {
    initSurface();
} 

VulkanSurface::~VulkanSurface(){
    vkDestroySurfaceKHR(vulkanInstance.getInstance(), surface, nullptr);
}

void VulkanSurface::initSurface() {
    surface = window.createSurface(vulkanInstance.getInstance());
}

} // namespace vkrt