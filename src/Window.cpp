// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include "vkrt/Window.hpp"

namespace vkrt {

Window::Window(int width, int height, const std::string& title) {
    initWindow(width, height, title);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window); }

void Window::pollEvents() const { glfwPollEvents(); }

std::vector<const char*> Window::getRequiredInstanceExtensions() {
    uint32_t count;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);
    return extensions;
}

VkSurfaceKHR Window::createSurface(VkInstance instance) const {
    VkSurfaceKHR surface;
    glfwCreateWindowSurface(instance, window, nullptr, &surface);
    return surface;
}

void Window::initWindow(int width, int height, const std::string& title) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
}

} // namespace vkrt