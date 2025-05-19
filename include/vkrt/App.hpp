// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_APP_HPP
#define VKRT_APP_HPP

#include "vkrt/Renderer.hpp"
#include "vkrt/Window.hpp"

namespace vkrt {

class App{
public:
    App();
    void run();

private:
    Window window;
    Renderer renderer;

    void init();
    void mainLoop();
    void cleanup();
};

} // namespace vkrt

#endif // VKRT_APP_HPP