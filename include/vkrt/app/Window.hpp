// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#ifndef VKRT_WINDOW_HPP
#define VKRT_WINDOW_HPP

#include <string>
#include <vector>

#include <volk.h>
#include <GLFW/glfw3.h>

namespace vkrt {

typedef void (* WindowDropFunc)(void* userdata, int count, const char** paths);

/**
 * @class Window
 * @brief Manages an application window using GLFW with setup for Vulkan rendering.
 * 
 * Handles window creation, event polling, and platform specific surface creation.
 */
class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void setDropCallback(WindowDropFunc callback);
    void setUserData(void* data);
    void pollEvents() const;

    std::vector<const char*> getRequiredInstanceExtensions();
    VkSurfaceKHR createSurface(VkInstance instance) const;

    bool shouldClose() const;
    GLFWwindow* getGLFWwindow() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getFramebufferWidth() const { return framebufferWidth; }
    int getFramebufferHeight() const { return framebufferHeight; }

private:
    static void internalDropCallback(GLFWwindow* window, int count, const char** paths);

    void initWindow(int width, int height, const std::string& title);

    GLFWwindow* window = nullptr;
    int width;
    int height;
    int framebufferWidth;
    int framebufferHeight;

    WindowDropFunc userSetDropCallback = nullptr;
    void* userData = nullptr;
};

} // namespace vkrt

#endif // VKRT_WINDOW_HPP