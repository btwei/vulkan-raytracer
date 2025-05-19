// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/core/VulkanSwapchain.hpp"

namespace vkrt {

VulkanSwapchain::VulkanSwapchain(const Window& window, const VulkanSurface& surface, const VulkanDevice& device) : window(window), vulkanSurface(surface), vulkanDevice(device) {
    setSwapchainProperties();
    createSwapchain();
    createSwapchainImageViews();
}

VulkanSwapchain::~VulkanSwapchain() {
    for(auto imageView : swapchainImageViews) {
        vkDestroyImageView(vulkanDevice.getDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(vulkanDevice.getDevice(), swapchain, nullptr);
}

void VulkanSwapchain::setSwapchainProperties() {
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanDevice.getPhysicalDevice(), vulkanSurface.getSurface(), &formatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanDevice.getPhysicalDevice(), vulkanSurface.getSurface(), &formatCount, formats.data());

    swapchainImageFormat = formats[0];
    for(const auto& format : formats) {
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            swapchainImageFormat = format;
            break;
        }
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanDevice.getPhysicalDevice(), vulkanSurface.getSurface(), &presentModeCount, nullptr);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanDevice.getPhysicalDevice(), vulkanSurface.getSurface(), &presentModeCount, presentModes.data());

    swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for(const auto& presentMode : presentModes) {
        if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR){
            swapchainPresentMode = presentMode;
            break;
        }
    }
}

void VulkanSwapchain::createSwapchain() {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanDevice.getPhysicalDevice(), vulkanSurface.getSurface(), &capabilities);

    swapchainExtent = chooseSwapchainExtent();

    uint32_t imageCount = capabilities.minImageCount + 1;
    if(capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount){
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vulkanSurface.getSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = swapchainImageFormat.format;
    createInfo.imageColorSpace = swapchainImageFormat.colorSpace;
    createInfo.imageExtent = swapchainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = { vulkanDevice.getComputeFamilyIndex(), vulkanDevice.getPresentFamilyIndex() };
    if(vulkanDevice.getComputeFamilyIndex() != vulkanDevice.getPresentFamilyIndex()){
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = swapchainPresentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(vulkanDevice.getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain!");
    }

    vkGetSwapchainImagesKHR(vulkanDevice.getDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vulkanDevice.getDevice(), swapchain, &imageCount, swapchainImages.data());
}

void VulkanSwapchain::createSwapchainImageViews() {
    swapchainImageViews.resize(swapchainImages.size());

    for(size_t i=0; i < swapchainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapchainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapchainImageFormat.format;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(vulkanDevice.getDevice(), &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to create image view!");
        }
    }
}

void VulkanSwapchain::recreateSwapchain() {
    
}

VkExtent2D VulkanSwapchain::chooseSwapchainExtent() {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanDevice.getPhysicalDevice(), vulkanSurface.getSurface(), &capabilities);
    
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(window.getFramebufferWidth()),
            static_cast<uint32_t>(window.getFramebufferHeight())
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

} //namespace vkrt