#ifndef VKRT_WINDOW_HPP
#define VKRT_WINDOW_HPP

#include <GLFW/glfw3.h>

namespace vkrt {

/**
 * @class Window
 * @brief Manages a platform window for rendering.
 * 
 * Handles window creation, event polling, and buffer swapping.
 */
class Window {
public:
    Window();
    ~Window();
    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    
private:

};

} // namespace vkrt

#endif // VKRT_WINDOW_HPP