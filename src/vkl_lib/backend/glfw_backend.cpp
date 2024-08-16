// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-implicit-bool-conversion, *-init-variables)
#include "vkl/backend/glfw_backend.hpp"

#include <vkl/timer/Timer.hpp>

void errorCallback(int error, const char *description) { LERROR("GLFW Error ({0}): {1}", error, description); }
void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    switch(key) {  // NOLINT(*-multiway-paths-covered)
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            LINFO("escape close");
        }
        break;
    // Add more cases for other keys if needed
    [[likely]] default:
        // Handle other keys here
        break;
    }
}
void setHints() {
    const vnd::AutoTimer timer("set glfw hints");
    // Set GLFW context version and profile
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}
void initializeGLFW() {
    const vnd::AutoTimer timer("glfw setup");
    if(!glfwInit()) {
        LCRITICAL("Failed to initialize GLFW");
        throw std::runtime_error("Failed to initialize GLFW.");
    }
    if(!glfwVulkanSupported()) {
        glfwTerminate();
        LCRITICAL("Failed to initialize GLFW. vulkan not supported");
        throw std::runtime_error("Failed to initialize GLFW. Vulkan not supported");
    }
    glfwSetErrorCallback(errorCallback);
}
std::string formatMode(const GLFWvidmode *mode) {
    return FORMAT("({}x{}, Bits rgb{}{}{}, RR:{}Hz)", mode->width, mode->height, mode->redBits, mode->greenBits, mode->blueBits,
                  mode->refreshRate);
}

GLFWwindow *createWindow(int width, int height, std::string_view name) {
    const vnd::AutoTimer timer("window creation");
    GLFWwindow *window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwSetKeyCallback(window, keyCallback);
    // glfwSetWindowUserPointer(window, this);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void centerWindow(GLFWwindow *window, int width, int height, std::string_view name) {
    vnd::Timer monitort("get primary Monitor");
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if(!primaryMonitor) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to get the primary monitor.");
    }
    LINFO("{}", monitort);
    vnd::Timer modet("get mode");
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
    if(!mode) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to get the video mode of the primary monitor.");
    }
    LINFO("{}", modet);
    vnd::Timer crepositiont("calculating for reposition");
    const int monitorWidth = mode->width;
    const int monitorHeight = mode->height;
    int windowWidth;
    int windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    auto centerX = CALC_CENTRO(monitorWidth, windowWidth);
    auto centerY = CALC_CENTRO(monitorHeight, windowHeight);
    LINFO("{}", crepositiont);
    vnd::Timer wrepositiont("window reposition");
    glfwSetWindowPos(window, centerX, centerY);
    int posX = 0;
    int posY = 0;
    glfwGetWindowPos(window, &posX, &posY);
    if(posX != centerX || posY != centerY) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to position the window at the center.");
    }
    LINFO("{}", wrepositiont);
    int xPos;
    int yPos;
    float xScale;
    float yScale;
    int monitorPhysicalWidth;
    int monitorPhysicalHeight;
    vnd::Timer tmonitorinfo("get monitor info");
    glfwGetMonitorPos(primaryMonitor, &xPos, &yPos);
    glfwGetMonitorContentScale(primaryMonitor, &xScale, &yScale);
    glfwGetMonitorPhysicalSize(primaryMonitor, &monitorPhysicalWidth, &monitorPhysicalHeight);
    LINFO("{}", tmonitorinfo);
    // glfwSetWindowUserPointer(window, this);
    // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwShowWindow(window);
    LINFO("Monitor:\"{}\", Phys:{}x{}mm, Scale:({}/{}), Pos:({}/{})", glfwGetMonitorName(primaryMonitor), monitorPhysicalWidth,
          monitorPhysicalHeight, xScale, yScale, xPos, yPos);
    LINFO("Monitor Mode:{}", formatMode(mode));
    LINFO("created the window {0}: (w: {1}, h: {2}, pos:({3}/{4}))", name.data(), width, height, centerX, centerY);
}
GLFWwindow *initWindow(int width, int height, std::string_view name) {
    initializeGLFW();
    setHints();
    auto *window = createWindow(width, height, name);
    centerWindow(window, width, height, name);
    return window;
}

GLFWwindow *build_window(int width, int height, std::string_view name) {
    return initWindow(width, height, name);
}

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-implicit-bool-conversion, *-init-variables)