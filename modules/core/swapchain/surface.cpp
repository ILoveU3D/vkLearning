#include <Application.h>

void Application::createSurface(){
    if(glfwCreateWindowSurface(instance, window, nullptr, &surface)!=VK_SUCCESS){
        throw std::runtime_error("failed to create surface");
    }
}