#ifndef VKRT_RENDERER_HPP
#define VKRT_RENDERER_HPP

#include "vkrt/Window.hpp"
#include "vkrt/core/VulkanInstance.hpp"
#include "vkrt/core/VulkanSurface.hpp"
#include "vkrt/core/VulkanDevice.hpp"

namespace vkrt {

class Renderer {
public:
    Renderer(Window& window);
private:
    VulkanInstance instance;
    VulkanSurface surface;
    VulkanDevice device;
};

} // namespace vkrt

#endif //VKRT_RENDERER_HPP