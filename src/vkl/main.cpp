// NOLINTBEGIN(*-include-cleaner)
#include <vkl/vkl.hpp>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `vkl`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

// NOLINTBEGIN(*-owning-memory)
// NOLINTNEXTLINE(bugprone-exception-escape)
void spawn_render_thread(GLFWwindow *window, std::atomic<bool> *done) {
    auto *engine = new Engine(window);

    while(!*done) {
        // Do work repeatedly!
    }

    delete engine;
}

int main() {
    INIT_LOG()
    LINFO("{} {}v", vkl::cmake::project_name, vkl::cmake::project_version);
    LINFO("{}", glfwGetVersionString());
    try {
        // NOLINTBEGIN(*-avoid-magic-numbers,*-magic-numbers)
        int width = 800;
        int height = 600;
        // NOLINTEND(*-avoid-magic-numbers,*-magic-numbers)
        GLFWwindow *window = build_window(width, height, WTITILE);

        std::atomic<bool> done = false;
        std::thread render_thread(spawn_render_thread, window, &done);
        auto *app = new App(window);

        done = true;
        render_thread.join();
        glfwTerminate();
        delete app;
        return 0;
    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
}

// NOLINTEND(*-owning-memory)
// NOLINTEND(*-include-cleaner)