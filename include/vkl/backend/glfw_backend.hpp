#pragma once
#include "../headers.hpp"
#include <GLFW/glfw3.h>

void errorCallback(int error, const char *description);
void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods);
void setHints();
void initializeGLFW();
std::string formatMode(const GLFWvidmode *mode);
GLFWwindow *createWindow(int width, int height, std::string_view name);
void centerWindow(GLFWwindow *window, int width, int height, std::string_view name);
GLFWwindow *initWindow(int width, int height, std::string_view name);

/**
 * @brief Build a window for our app.
 *
 * @param width width of the window in pixels
 * @param height height of the window in pixels
 * @param name name of the window
 * @return GLFWwindow* The created window
 */
GLFWwindow *build_window(int width, int height, std::string_view name);