// 创建窗口

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <Application.h>

bool firstMouseFlag = true;

void Application::framebufferResizeCallback(GLFWwindow* window, int width, int height){
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app -> framebufferResized = true;
}

void Application::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Application::mouseCallback(GLFWwindow* window, double xposIn, double yposIn){
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    app -> camera.responseMouseMovement(xpos, ypos);

}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app -> camera.responseMouseScroll(static_cast<float>(yoffset));
}

void Application::initWindow(){
    std::cout << "initializing glfw Window..." << std::endl;
    glfwInit();
    // 不要创建带有opengl的上下文
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if(monitor == NULL)
        throw std::runtime_error("primary monitor not found!");
    // 创建窗口
#ifndef MOORETHREADS
    window = glfwCreateWindow(mode->width, mode->height, "Viking's Room!", monitor, nullptr);
#else
    window = glfwCreateWindow(800, 800, "Viking's Room!", nullptr, nullptr);
#endif
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    std::cout << "Window Complete" << std::endl;
}