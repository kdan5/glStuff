#ifndef GLINIT_H
#define GLINIT_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

// graphics settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void glfwSetup();
GLFWwindow* createWindow(const char* title);
bool glLoad();

#endif

