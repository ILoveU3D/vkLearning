#include <Application.h>

// 创建实例
void Application::createInstance(){
    // 检查验证层可用性
    if(enableValidationLayers && !checkValidationLayerSupport())
        throw std::runtime_error("valiation layer not avaliable!");

    // Vulkan避免初始化参数过多的方式，结构体式的定义
    // 应用程序相关信息
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Wyk triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_API_VERSION_1_0;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // 实例信息，包括需要指定的全局扩展
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }else{
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // 所有信息填写完毕，创建实例
    if(vkCreateInstance(&createInfo, nullptr, &instance)!=VK_SUCCESS){
        throw std::runtime_error("failed to create instance!");
    }
    // 查看可用的拓展
    for(auto extension:extensions)
        std::cout << "Avaliable extension:" << extension << std::endl;
}