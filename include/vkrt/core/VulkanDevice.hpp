// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_VULKANDEVICE_HPP
#define VKRT_VULKANDEVICE_HPP

#include <map>
#include <set>
#include <vector>

#include <volk.h>

#include "vkrt/core/VulkanInstance.hpp"
#include "vkrt/core/VulkanSurface.hpp"

namespace vkrt {

/**
 * @class VulkanDevice
 * @brief Encapuslates physical device selection and creation of a logical device with necessary queues.
 * 
 * This class handles the vulkan physical device selection and logical device creation. It selects the
 * best physical device to run on based on required extensions and features. It then creates a logical
 * device with a compute queue and a present queue (which are not guaranteed to be the same).
 */
class VulkanDevice {
public:
    VulkanDevice(VulkanInstance& instance, VulkanSurface& surface);
    ~VulkanDevice();

    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;

    VkDevice getDevice() const { return device; }
    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
    uint32_t getComputeFamilyIndex() const { return computeFamilyIndex; }
    uint32_t getPresentFamilyIndex() const { return presentFamilyIndex; }
    VkQueue getComputeQueue() const { return computeQueue; }
    VkQueue getPresentQueue() const { return presentQueue; }

private:
    static constexpr char* requiredExtensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    };
    static constexpr size_t requiredExtensionCount = sizeof(requiredExtensions) / sizeof(requiredExtensions[0]);

    const VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    uint32_t computeFamilyIndex;
    uint32_t presentFamilyIndex;
    VkQueue computeQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    void pickPhysicalDevice(VkSurfaceKHR surface);
    int rateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
    void findQueueFamilies(VkSurfaceKHR surface);
    void createLogicalDevice();
};

} // namespace vkrt

#endif // VKRT_VULKANDEVICE_HPP