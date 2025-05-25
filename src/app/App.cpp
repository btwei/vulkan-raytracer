// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/app/App.hpp"

vkrt::App::App() : window(800, 600, "Vulkan Raytracer"),
                   renderer(window) {

}

void vkrt::App::run() {
    init();
    mainLoop();
    cleanup();
}

void vkrt::App::init() {

}

void vkrt::App::mainLoop() {
    while (!window.shouldClose()) {
        window.pollEvents();
    }
}

void vkrt::App::cleanup() {

}