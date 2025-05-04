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