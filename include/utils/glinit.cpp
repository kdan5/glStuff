#include "glinit.h"

void setupGLFWContext() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* createWindow(const char* title = "") {
    // create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to initialize GLFW window." << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    return window;
}

bool glLoad() {
    return (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

