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
#include "vkrt/renderer/ResourceManager.hpp"

namespace vkrt {

struct RenderableState {

};

/**
 * @class Renderer
 * @brief A top-level rendering class for raytracing and multi-threaded rendering.
 * 
 * This class implements all rendering related functions of vkrt. This class is 
 * designed to run in a dedicated rendering thread calling renderFrame(). In a 
 * separate update thread, functions such as loadMesh or updateState can be called.
 */
class Renderer {
public:
    Renderer(Window& window);

    void renderFrame();
    void updateState(RenderableState renderState); // thread-safe

    MeshHandle loadOBJ(const char* path); // thread-safe
    void unloadModel(MeshHandle meshHandle); // thread-safe

private:
    VulkanInstance instance;
    VulkanSurface surface;
    VulkanDevice device;
    VulkanSwapchain swapchain;
    VulkanCommandPool graphicsPool;
    VulkanCommandPool computePool;
    VulkanCommandPool singleTimePool;

    VulkanAllocator allocator;
    ResourceManager resourceManager;

    RenderableState queuedState;
    RenderableState currentState;

};

} // namespace vkrt

#endif //VKRT_RENDERER_HPP