#include "vkrt/Renderer.hpp"

namespace vkrt {

Renderer::Renderer(Window& window) : instance(window),
                                     surface(window, instance),
                                     device(instance, surface),
                                     swapchain(window, surface, device) {

}

} // namespace vkrt