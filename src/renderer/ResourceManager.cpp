// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/renderer/ResourceManager.hpp"

namespace vkrt {

ResourceManager::ResourceManager(const VulkanAllocator& vulkanAllocator) 
: vulkanAllocator(vulkanAllocator) {
    // Reserve the 0th idx
    meshList.push_back(nullptr); 
}

ResourceManager::~ResourceManager() {

}

MeshHandle ResourceManager::loadOBJ(const char* path) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path)) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for(const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if(attrib.normals.empty()){
                // If normals are missing
                vertex.normal = {0.0f, 0.0f, 0.0f};
            } else {
                vertex.normal = {
                    attrib.normals[3 * index.vertex_index + 0],
                    attrib.normals[3 * index.vertex_index + 1],
                    attrib.normals[3 * index.vertex_index + 2]
                };
            }

            vertex.uv = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.tangent = {0.0f, 0.0f, 0.0f, 0.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    vkrt::MeshBuffer gpuBuf = vulkanAllocator.uploadMesh(vertices, indices);

    std::lock_guard<std::mutex> lock(meshListMutex);
    if(!freeList.empty()) {
        size_t idx = freeList.back();
        freeList.pop_back();
        meshList[idx] = std::make_unique<MeshData>(std::move(gpuBuf));
        return MeshHandle{static_cast<uint32_t>(idx)};
    } else {
        size_t idx = meshList.size();
        if(idx >= std::numeric_limits<uint32_t>::max()) throw std::runtime_error("MeshList overflow! Please destroy meshes before adding more!");
        meshList.push_back(std::make_unique<MeshData>(std::move(gpuBuf)));
        return MeshHandle{static_cast<uint32_t>(idx)};
    }
}

// TODO: remove all freeList entries that are at the end of meshList to reduce meshList size 
void ResourceManager::unloadMesh(MeshHandle meshHandle) {
    if(meshHandle.id == 0) return;
    std::lock_guard<std::mutex> lock(meshListMutex);
    meshList[meshHandle.id].reset();
    freeList.push_back(meshHandle.id);
}

} // namespace vkrt