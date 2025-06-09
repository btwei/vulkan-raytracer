// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include <filesystem>

#include "vkrt/app/Engine.hpp"

namespace vkrt {

Engine::Engine(Window& window, Renderer& renderer) 
: window(window)
, renderer(renderer) { 
    window.setUserData(this);
    window.setDropCallback(dropCallback);
}

Engine::~Engine() {

}

void Engine::run() {
    while (!window.shouldClose()) {

        window.pollEvents();
    }
}

void Engine::dropCallback(void* userdata, int count, const char** paths) {
    Engine* engine = reinterpret_cast<Engine*>(userdata);

    for(int i=0; i < count; i++){
        // Handle .obj files
        if(static_cast<std::filesystem::path>(paths[i]).extension() == ".obj") {
            MeshHandle handle = engine->renderer.loadOBJ(paths[i]);

            //Do error checking
            if(handle.id == 0) continue;

            //TODO: Store handle
            std::cout << handle.id << std::endl;
        }
    }
}

} // namespace vkrt