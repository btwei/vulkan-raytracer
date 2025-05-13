#ifndef VKRT_RENDERER_HPP
#define VKRT_RENDERER_HPP

#include "vkrt/Window.hpp"
#include "vkrt/core/VulkanInstance.hpp"
#include "vkrt/core/VulkanSurface.hpp"
#include "vkrt/core/VulkanDevice.hpp"
#include "vkrt/core/VulkanSwapchain.hpp"

namespace vkrt {

class Renderer {
public:
    Renderer(Window& window);
private:
    VulkanInstance instance;
    VulkanSurface surface;
    VulkanDevice device;
    VulkanSwapchain swapchain;
};

} // namespace vkrt

#endif //VKRT_RENDERER_HPP