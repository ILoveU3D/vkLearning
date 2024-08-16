#include <Application.h>

// 按理说要对Physical device的graphic性能进行严格检查，但是我们信任MT Device，忽略这一步
void Application::pickPhysicalDevice(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    vkEnumeratePhysicalDevices(instance, &deviceCount, &physicalDevice);

    if(physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("failed to find a physical GPU");
    }
}