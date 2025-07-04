// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANSWAPCHAIN_HPP
#define VKRT_VULAKNSWAPCHAIN_HPP

#include <algorithm>
#include <cstdint>
#include <limits>

#include "vkrt/core/VulkanSurface.hpp"
#include "vkrt/core/VulkanDevice.hpp"
#include "vkrt/app/Window.hpp"

#include <volk.h>

namespace vkrt {

/**
 * @class VulkanSwapchain
 * @brief Handles swapchain creation, recreation, and destruction
 * 
 * This class encapsulates swapchain handling. It queries for optimal swapchain
 * properties and then creates one. In the event of a window resize, recreateSwapchain()
 * will rebuild the swapchain with the correct extent2D. This class also provides access
 * to the swapchain's images via getSwapchainImages().
 */
class VulkanSwapchain {
public:
    VulkanSwapchain(const Window& window, const VulkanSurface& surface, const VulkanDevice& device);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

    void recreateSwapchain();

    VkSwapchainKHR getSwapchain() const { return swapchain; }
    std::vector<VkImage> getSwapchainImages() const { return swapchainImages; }

private:
    const Window& window;
    const VulkanSurface& vulkanSurface;
    const VulkanDevice& vulkanDevice;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;

    VkSurfaceFormatKHR swapchainImageFormat;
    VkExtent2D swapchainExtent;
    VkPresentModeKHR swapchainPresentMode;

    void setSwapchainProperties();
    void createSwapchain();
    void createSwapchainImageViews();

    VkExtent2D chooseSwapchainExtent();
};

} // namespace vkrt

#endif // VKRT_VULAKNSWAPCHAIN_HPP