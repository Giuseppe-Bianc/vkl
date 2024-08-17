// NOLINTBEGIN(*-include-cleaner)
#include "vkl/renderer/renderer.hpp"

Engine::Engine(GLFWwindow *window) : /*logger(Logger::get_logger())*/ window(window) {
    LINFO("Made a graphics engine");

    instance = make_instance("Real Engine", deletionQueue);
    dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
    debugMessenger = make_debug_messenger(instance, dldi, deletionQueue);
}

Engine::~Engine() {
    LINFO("Goodbye see you!");
    while(!deletionQueue.empty()) {
        deletionQueue.back()(instance);
        deletionQueue.pop_back();
    }
}

// NOLINTEND(*-include-cleaner)