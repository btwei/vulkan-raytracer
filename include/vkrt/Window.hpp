#ifndef VKRT_WINDOW_HPP
#define VKRT_WINDOW_HPP

#include <string>
#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace vkrt {

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

    bool shouldClose() const;
    void pollEvents() const;

    std::vector<const char*> getRequiredInstanceExtensions();
    VkSurfaceKHR createSurface(VkInstance instance) const;

    GLFWwindow* getGLFWwindow() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getFramebufferWidth() const { return framebufferWidth; }
    int getFramebufferHeight() const { return framebufferHeight; }

private:
    void initWindow(int width, int height, const std::string& title);

    GLFWwindow* window = nullptr;
    int width;
    int height;
    int framebufferWidth;
    int framebufferHeight;
};

} // namespace vkrt

#endif // VKRT_WINDOW_HPP