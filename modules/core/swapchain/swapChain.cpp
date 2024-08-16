#include <Application.h>
#include <SwapChainSupport.h>

void Application::createSwapChain(){
    // 查询交换链能力
    SwapChainSupport details;
    // 首先查询基本表面功能
    // 最大图像大小2560*1440，因此要根据显示器大小进行裁剪
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);
    VkSurfaceFormatKHR surfaceFormat = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    VkExtent2D extent = details.capabilities.currentExtent;

    // 指定交换链中拥有图像数量
    uint32_t imageCount = details.capabilities.minImageCount + 1;
    if(details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
        imageCount = details.capabilities.maxImageCount;
    
    // 创建交换链
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain)!=VK_SUCCESS)
        throw std::runtime_error("failed to create swap chain");

    // 存储其他临时变量
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void Application::cleanupSwapChain(){
    for(size_t i = 0;i < swapChainFrameBuffers.size();i++)
        vkDestroyFramebuffer(device, swapChainFrameBuffers[i], nullptr);
    for(size_t i = 0;i < swapChainFrameBuffers.size();i++)
        vkDestroyImageView(device, swapChainImageViews[i], nullptr);
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void Application::recreateSwapChain(){
    // 处理最小化
    int width = 0,height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while(width == 0 || height == 0){
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device);
    cleanupSwapChain();
    createSwapChain();
    createImageViews();
    createFramebuffers();
}