#ifndef VKRT_RENDERER_HPP
#define VKRT_RENDERER_HPP

#include "vkrt/Window.hpp"
#include "vkrt/core/VulkanInstance.hpp"

namespace vkrt {

class Renderer {
public:
    Renderer(Window& window);
private:
    VulkanInstance instance;
};

} // namespace vkrt

#endif //VKRT_RENDERER_HPP