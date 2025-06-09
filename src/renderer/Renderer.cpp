// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/renderer/Renderer.hpp"

namespace vkrt {

Renderer::Renderer(Window& window) : instance(window),
                                     surface(window, instance),
                                     device(instance, surface),
                                     swapchain(window, surface, device),
                                     graphicsPool(device, static_cast<VkCommandPoolCreateFlags>(0), device.getPresentFamilyIndex()),
                                     computePool(device, static_cast<VkCommandPoolCreateFlags>(0), device.getComputeFamilyIndex()),
                                     singleTimePool(device, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, device.getPresentFamilyIndex()),
                                     allocator(instance, device, singleTimePool),
                                     resourceManager(allocator) {

}

MeshHandle Renderer::loadOBJ(const char* path) {
    return resourceManager.loadOBJ(path);
}

void Renderer::unloadModel(MeshHandle meshHandle){
    resourceManager.unloadMesh(meshHandle);
}

} // namespace vkrt