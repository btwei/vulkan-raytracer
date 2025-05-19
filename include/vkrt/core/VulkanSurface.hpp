// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANSURFACE_HPP
#define VKRT_VULKANSURFACE_HPP

#include "vkrt/Window.hpp"
#include "vkrt/core/VulkanInstance.hpp"

namespace vkrt {

/**
 * @class VulkanSurface
 * @brief Encapsulates surface creation and destruction.
 * 
 * Surface creation is implemented by the Window class, which separates Vulkan components from
 * a specific windowing library.
 */
class VulkanSurface {
public:
    VulkanSurface(const Window& window, const VulkanInstance& instance);
    ~VulkanSurface();

    VulkanSurface(const VulkanSurface&) = delete;
    VulkanSurface& operator=(const VulkanSurface&) = delete;

    VkSurfaceKHR getSurface() const { return surface; }

private:
    const Window& window;
    const VulkanInstance& vulkanInstance;

    VkSurfaceKHR surface;

    void initSurface();
};

} // namespace vkrt

#endif // VKRT_VULKANSURFACE_HPP