// NOLINTBEGIN(*-include-cleaner)
#include "vkl/app.hpp"
#include <vkl/FPSCounter.hpp>
// #include <vkl/timer/Timer.hpp>

namespace lve {
    void App::run() {
        // Loop principale della finestra
        FPSCounter fps_counter{window.getGLFWWindow(), WTITILE};
        while(!window.shouldClose()) {
            // Polling degli eventi
            glfwPollEvents();
            fps_counter.frameInTitle();
        }
    }
}  // namespace lve
   // NOLINTEND(*-include-cleaner)