#include "vkrt/App.hpp"

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