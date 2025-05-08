#include "vkrt/core/VulkanSwapchain.hpp"

namespace vkrt {

VulkanSwapchain::VulkanSwapchain(VulkanSurface& surface, VulkanDevice& device) : device(device.getDevice()) {

}

VulkanSwapchain::~VulkanSwapchain() {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
}

void VulkanSwapchain::initSwapchain() {

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain!");
    }
}

} //namespace vkrt