#include <HelloTriangleApplication.h>

void HelloTriangleApplication::createSyncObjects(){
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for(size_t i = 0;i < MAX_FRAMES_IN_FLIGHT;i++){
        if(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphores[i])!=VK_SUCCESS)
        throw std::runtime_error("failed to create semaphore of swap chain");
        if(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderFinishSemaphores[i])!=VK_SUCCESS)
            throw std::runtime_error("failed to create semaphore of render pass");
        if(vkCreateFence(device, &fenceCreateInfo, nullptr, &inFlightFences[i])!=VK_SUCCESS)
            throw std::runtime_error("failed to create fence of flight");
    }
}