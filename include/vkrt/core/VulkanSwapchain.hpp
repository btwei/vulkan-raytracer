#ifndef VKRT_VULKANSWAPCHAIN_HPP
#define VKRT_VULAKNSWAPCHAIN_HPP

#include "vkrt/core/VulkanSurface.hpp"
#include "vkrt/core/VulkanDevice.hpp"

#include <vulkan/vulkan.h>

namespace vkrt {

class VulkanSwapchain {
public:
    VulkanSwapchain(VulkanSurface& surface, VulkanDevice& device);
    ~VulkanSwapchain();

    VulkanSwapchain(const VulkanSwapchain&) = delete;
    VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

    VkSwapchainKHR getSwapchain() const { return swapchain; }

private:
    const VkDevice device;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;

    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;

    void initSwapchain();
};

} // namespace vkrt

#endif // VKRT_VULAKNSWAPCHAIN_HPP