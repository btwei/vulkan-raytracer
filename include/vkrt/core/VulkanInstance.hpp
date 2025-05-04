#ifndef VKRT_VULKANINSTANCE_HPP
#define VKRT_VULKANINSTANCE_HPP

#include <stdexcept>

#include <vulkan/vulkan.h>

#include "vkrt/Window.hpp"

namespace vkrt {

class VulkanInstance {
public:
    VulkanInstance(Window& window);
    ~VulkanInstance();

    VkInstance getInstance() { return instance; }
private:
    void initInstance(Window& window);

    VkInstance instance;
};

} // namespace vkrt

#endif // VKRT_VULKANINSTANCE_HPP