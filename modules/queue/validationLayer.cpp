#include <Application.h>

// 由于vkCreateDebugUtilsMessengerEXT扩展函数并不能自动加载，因此我们需要首先定义它的调用与销毁
VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger){
    auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    return function!=nullptr?function(instance, pCreateInfo, pAllocator, pDebugMessenger):VK_ERROR_EXTENSION_NOT_PRESENT;
}

// 检查我们定义的验证层是否可用
bool Application::checkValidationLayerSupport(){
    // 经典的vulkan两次调用查询
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(auto layer:validationLayers){
        bool found = false;
        for(auto property:availableLayers){
            if(strcmp(layer, property.layerName)==0){
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }
    return true;
}

// 得到回调信使的拓展
std::vector<const char*> Application::getRequiredExtensions(){
    uint32_t glfwExtensionCount = 0;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions+glfwExtensionCount);
    if(enableValidationLayers){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

// vulkan标准回调函数
// 参数一代表消息的严重性，参数二代表消息的值，参数三包含了消息的详细信息
VKAPI_ATTR VkBool32 VKAPI_CALL Application::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
    std::cerr << "\x1B[32m" << "[Validation Layer]" << "\x1B[0m" << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

// 为了同时在用例创建和销毁是实现对这两个过程的验证
void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

// 设置回调信使
void Application::setupDebugMessenger(){
    if(!enableValidationLayers)
        return;
    
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    // 调用我们自己用ProcAddr得到的create函数
    if(createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger)!=VK_SUCCESS){
        throw std::runtime_error("fail to set up debug messenger");
    }
}