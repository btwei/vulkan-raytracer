#include "vkrt/core/VulkanDevice.hpp"

namespace vkrt {

VulkanDevice::VulkanDevice(VulkanInstance& instance, VulkanSurface& vulkanSurface) : instance(instance.getInstance()) {
    pickPhysicalDevice(vulkanSurface.getSurface());
    findQueueFamilies(vulkanSurface.getSurface());
    createLogicalDevice();
}

VulkanDevice::~VulkanDevice() {
    vkDestroyDevice(device, nullptr);
}

void VulkanDevice::pickPhysicalDevice(VkSurfaceKHR surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device, surface);
        candidates.insert(std::make_pair(score, device));
    }

    // Check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

int VulkanDevice::rateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    int score = 0;

    // GPU must support all required extensions
    for(char* required : requiredExtensions) {
        bool extensionFound = false;
        for(VkExtensionProperties available : availableExtensions){
            if(strcmp(required, available.extensionName) == 0){
                extensionFound = true;
                break;
            }
        }
        if(extensionFound == false) return 0;
    }
    
    // GPU must possess a queue family with compute support and one with present support
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    bool computeFamilyFound = false;
    bool presentFamilyFound = false;

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) computeFamilyFound = true;
        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if(presentSupport) presentFamilyFound = true;

        i++;
    }

    if(!computeFamilyFound || !presentFamilyFound) return 0;

    // Prefer discrete gpus (which tend to have better performance)
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    return score;
}

void VulkanDevice::findQueueFamilies(VkSurfaceKHR surface) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    bool computeFamilyFound = false;
    bool presentFamilyFound = false;

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if(!computeFamilyFound && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            computeFamilyIndex = i;
            computeFamilyFound = true;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if(!presentFamilyFound && presentSupport) {
            presentFamilyIndex = i;
            presentFamilyFound = true;
        }

        i++;
    }
}

void VulkanDevice::createLogicalDevice() {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { computeFamilyIndex, presentFamilyIndex };

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = requiredExtensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeatures{};
    accelFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    accelFeatures.accelerationStructure = VK_TRUE;

    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingFeatures{};
    rayTracingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    rayTracingFeatures.pNext = &accelFeatures;
    rayTracingFeatures.rayTracingPipeline = VK_TRUE;

    createInfo.pNext = &rayTracingFeatures;

    if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, computeFamilyIndex, 0, &computeQueue);
    vkGetDeviceQueue(device, presentFamilyIndex, 0, &presentQueue);
}

} // namespace vkrt