#include <iostream>
#include <stdlib.h>
#include <exception>

#include "vkrt/App.hpp"

int main() {
    try {
        vkrt::App app;
        app.run();
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}