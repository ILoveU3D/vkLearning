// 创建窗口

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <Application.h>

void Application::framebufferResizeCallback(GLFWwindow* window, int width, int height){
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app -> framebufferResized = true;
}

void escapeCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::initWindow(){
    std::cout << "initializing glfw Window..." << std::endl;
    glfwInit();
    // 不要创建带有opengl的上下文
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    // 创建窗口
    window = glfwCreateWindow(mode->width, mode->height, "Viking's Room!", monitor, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, escapeCallback);
    std::cout << "Window Complete" << std::endl;
}