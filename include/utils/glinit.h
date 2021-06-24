#ifndef GLINIT_H
#define GLINIT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// graphics settings
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void glfwSetup();
GLFWwindow* createWindow(const char* title);
bool glLoad();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

#endif

