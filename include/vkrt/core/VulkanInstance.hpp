// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANINSTANCE_HPP
#define VKRT_VULKANINSTANCE_HPP

#include <cstring>
#include <iostream>
#include <stdexcept>

#include <volk.h>

#include "vkrt/app/Window.hpp"

namespace vkrt {

/**
 * @class VulkanInstance
 * @brief Encapsulates the creation, management, and destruction of a Vulkan instance.
 * 
 * This class handles initialization of the Vulkan instance, including application
 * info, required extensions, validation layers, and debug messenger (if enabled). 
 * It ensures proper cleanup of the Vulkan instance resources when no longer needed.
 */
class VulkanInstance {
public:
    VulkanInstance(Window& window);
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

    VkInstance getInstance() const { return instance; }

private:
    static constexpr const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };
    static constexpr size_t layerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

    void initInstance(Window& window);

    bool checkValidationLayerSupport();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
};

} // namespace vkrt

#endif // VKRT_VULKANINSTANCE_HPP