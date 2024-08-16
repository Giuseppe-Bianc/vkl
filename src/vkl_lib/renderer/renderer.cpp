// NOLINTBEGIN(*-include-cleaner)
#include "vkl/renderer/renderer.hpp"

Engine::Engine(GLFWwindow *window) : window(window) {
    LINFO("Made a graphics engine");
    instance = make_instance("Real Engine", deletionQueue);
}

Engine::~Engine() {
    LINFO("Goodbye see you!");
    while (!deletionQueue.empty()) {
        deletionQueue.back()();
        deletionQueue.pop_back();
    }
}

// NOLINTEND(*-include-cleaner)