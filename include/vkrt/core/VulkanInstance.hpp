#ifndef VULKANINSTANCE_HPP
#define VULKANINSTANCE_HPP

#include <vulkan/vulkan.h>

class VulkanInstance {
public:
    VulkanInstance();
    ~VulkanInstance();

    VkInstance getInstance() { return instance; }
private:
    VkInstance instance;
};

#endif