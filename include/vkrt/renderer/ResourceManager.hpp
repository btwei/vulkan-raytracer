// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_RESOURCEMANAGER_HPP
#define VKRT_RESOURCEMANAGER_HPP

#include <cstddef>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "tiny_obj_loader.h"

#include "vkrt/core/VulkanAllocator.hpp"

namespace vkrt {

struct MeshHandle { 
    uint32_t id = 0;

    MeshHandle() = default;
    bool is_valid() { return id!=0; }
};

/**
 * @class ResourceManager
 * @brief Loads and manages gpu sided objects.
 * 
 * This class takes a file in and returns a user friendly handle to reference later. This
 * class formats a mesh file into a standard vertex format and then uses VulkanAllocator to
 * upload it to the GPU.
 */
class ResourceManager {
public:
    ResourceManager(const VulkanAllocator& vulkanAllocator);
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    MeshHandle loadOBJ(const char* path);
    void unloadMesh(MeshHandle meshHandle);

private:
    struct MeshData {
        MeshBuffer gpuBuf;
        // BLAS ?

        MeshData(MeshBuffer&& meshBuffer) : gpuBuf(std::move(meshBuffer)) {}
    };

    const VulkanAllocator& vulkanAllocator;

    std::mutex meshListMutex; // This mutex guards BOTH meshList and freeList
    std::vector<std::unique_ptr<MeshData>> meshList;
    std::vector<size_t> freeList;

};

} // namespace vkrt

#endif // VKRT_RESOURCEMANAGER_HPP