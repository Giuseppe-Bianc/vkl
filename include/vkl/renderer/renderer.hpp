#pragma once
#include "../headers.hpp"
#include <GLFW/glfw3.h>
#include "instance.hpp"

/**
 * @brief Vroom vroom.
 *
 */
class Engine {
public:
    /**
     * @brief Construct a new Engine object
     *
     * @param window main window to render to
     */
    Engine(GLFWwindow *window);

    /**
     * @brief Destroy the Engine object
     *
     */
    ~Engine();

private:
    /**
     * @brief Main window
     *
     */
    GLFWwindow *window;

    /**
     *@brief Stores destructors
     */
    std::deque<std::function<void()>> deletionQueue;

    /**
     * @brief the main instance
     */
    vk::Instance instance;
};