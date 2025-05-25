// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_RENDERER_HPP
#define VKRT_RENDERER_HPP

#include "vkrt/app/Window.hpp"
#include "vkrt/core/VulkanInstance.hpp"
#include "vkrt/core/VulkanSurface.hpp"
#include "vkrt/core/VulkanDevice.hpp"
#include "vkrt/core/VulkanSwapchain.hpp"
#include "vkrt/core/VulkanCommandObjects.hpp"
#include "vkrt/core/VulkanAllocator.hpp"

namespace vkrt {

struct vkrtModel { 
    uint32_t id = 0;

};

struct RenderableState {

};

/**
 * @class Renderer
 * @brief
 * 
 * 
 */
class Renderer {
public:
    Renderer(Window& window);

    void renderFrame();
    void updateState(RenderableState renderState); // thread-safe
    vkrtModel loadModel(const char** path); // thread-safe
    void unloadModel(vkrtModel model); // thread-safe

private:
    VulkanInstance instance;
    VulkanSurface surface;
    VulkanDevice device;
    VulkanSwapchain swapchain;
    VulkanCommandPool graphicsPool;
    VulkanCommandPool computePool;

    RenderableState queuedState;
    RenderableState currentState;
    std::vector<> modelInfos;
};

} // namespace vkrt

#endif //VKRT_RENDERER_HPP