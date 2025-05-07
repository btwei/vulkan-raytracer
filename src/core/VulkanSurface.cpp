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