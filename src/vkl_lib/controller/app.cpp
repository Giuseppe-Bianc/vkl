// NOLINTBEGIN(*-include-cleaner)
#include "vkl/controller/app.hpp"

#include <vkl/FPSCounter.hpp>

App::App(GLFWwindow *window) : window(window) { main_loop(); }

void App::main_loop() {
    FPSCounter fps_counter{window, WTITILE};
    while(!glfwWindowShouldClose(window)) { // NOLINT(*-implicit-bool-conversion)
        glfwPollEvents();
        fps_counter.frameInTitle();
    }
}

// NOLINTEND(*-include-cleaner)