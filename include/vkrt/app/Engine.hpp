// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_ENGINE_HPP
#define VKRT_ENGINE_HPP

#include <iostream>

#include "vkrt/app/Window.hpp"
#include "vkrt/renderer/Renderer.hpp"

namespace vkrt {

class Engine {
public:
    Engine(Window& window, Renderer& renderer);
    ~Engine();

    void run();

private:
    static void dropCallback(void* userdata, int count, const char** paths);

    Window& window;
    Renderer& renderer;

}; 

} // namespace vkrt

#endif // VKRT_ENGINE_HPP