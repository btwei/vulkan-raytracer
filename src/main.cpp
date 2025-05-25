// Copyright (c) 2025 Benjamin Wei. All rights reserved.
// This file is part of the vulkan-raytracer project.
// Use, copying, modification, redistribution, or inclusion in training data (e.g. for AI) is strictly prohibited.

#include <iostream>
#include <stdlib.h>
#include <exception>

#include "vkrt/app/App.hpp"

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