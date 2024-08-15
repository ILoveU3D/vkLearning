#include <Application.h>
#include <UniformBufferObject.h>

// 渲染一帧包含以下步骤:
// 1.等待上一帧完成
// 2.从swap chain中获取图像
// 3.录制一个command buffer，这个command将其中的命令场景绘制到image中
// 4.提交录制好的command buffer
// 5.展示swap chain中的图像
// 以上的GPU步骤需要做好各级同步
void Application::drawFrame(){
    // 首先等待上一帧完成
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // 从swap chain中获取图像
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        throw std::runtime_error("failed to get swap chain image");
    else if(result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return;
    }

    inputCallback();
    updateUniformBuffer();

    // 此处避免死锁
    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    // 录制一个command buffer
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    // 提交command buffer
    // 表明等待的pipeline阶段与信号量
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    // 指定command buffer
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];
    // 指定command buffer完成后触发的信号量
    VkSemaphore signalSemaphores[] = {renderFinishSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    // 将command buffer提交到graphic queue中，并在完成该command buffer时启动fence
    if(vkQueueSubmit(graphicQueue, 1, &submitInfo, inFlightFences[currentFrame])!=VK_SUCCESS)
        throw std::runtime_error("failed to submit draw command buffer");

    // 将结果提交回swap chain，并显示
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;
    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized){
        framebufferResized = false;
        recreateSwapChain();
    }else if(result!=VK_SUCCESS)
        throw std::runtime_error("failed to present swap chain image");

    // 前进帧
    currentFrame = (currentFrame+1) % MAX_FRAMES_IN_FLIGHT;
}

void Application::updateUniformBuffer(){
    auto currentTime = static_cast<float>(glfwGetTime());
    deltaFrame = currentTime - lastTime;
    lastTime = currentTime;
    
    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), (currentTime - startTime) * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = camera.getViewMatrix();
    ubo.proj = glm::perspective(glm::radians(45.0f), static_cast<float>(swapChainExtent.width / swapChainExtent.height), 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}

void Application::inputCallback(){
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.responseKey(FORWARD, deltaFrame);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.responseKey(BACK, deltaFrame);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.responseKey(LEFT, deltaFrame);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.responseKey(RIGHT, deltaFrame);
}