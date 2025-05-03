#include <stdlib.h>
#include <exception>

#include <vkrt/App.hpp>

int main() {
    try {
        App app;
        app.run();
    } catch (const std::exception &e) {
        // Handle error here
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}