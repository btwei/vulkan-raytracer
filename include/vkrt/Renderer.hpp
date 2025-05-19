// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_RENDERER_HPP
#define VKRT_RENDERER_HPP

#include "vkrt/Window.hpp"
#include "vkrt/core/VulkanInstance.hpp"
#include "vkrt/core/VulkanSurface.hpp"
#include "vkrt/core/VulkanDevice.hpp"
#include "vkrt/core/VulkanSwapchain.hpp"
#include "vkrt/core/VulkanCommandObjects.hpp"

namespace vkrt {

class Renderer {
public:
    Renderer(Window& window);
private:
    VulkanInstance instance;
    VulkanSurface surface;
    VulkanDevice device;
    VulkanSwapchain swapchain;
    VulkanCommandPool graphicsPool;
    VulkanCommandPool computePool;
};

} // namespace vkrt

#endif //VKRT_RENDERER_HPP