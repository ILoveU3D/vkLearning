#include <array>

#include <HelloTriangleApplication.h>
#include <QueueFamilyIndice.h>

void HelloTriangleApplication::createCommandPool(){
    // 获取两个队列
    QueueFamilyIndice2 indice;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    uint32_t index = 0;
    for(const auto& queueFamily:queueFamilies){
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indice.graphicFamily = index;
        VkBool32 surfaceSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &surfaceSupport);
        if(surfaceSupport)
            indice.presentFamily = index;
        if(indice.isOK())
            break;
        index++;
    }
    
    // 创建command pool
    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = indice.graphicFamily.value();
    if(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool)!=VK_SUCCESS)
        throw std::runtime_error("failed to create command pool");
}

void HelloTriangleApplication::createCommandBuffer(){
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo commandBufferAllocInfo{};
    commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool = commandPool;
    commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    if(vkAllocateCommandBuffers(device, &commandBufferAllocInfo, commandBuffers.data())!=VK_SUCCESS)
        throw std::runtime_error("failed to allocate command buffer");
}

void HelloTriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex){
    // 开始录制缓冲区
    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags = 0;
    commandBufferBeginInfo.pInheritanceInfo = nullptr;
    if(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo)!=VK_SUCCESS)
        throw std::runtime_error("failed to begin recording command buffer");

    // 启动渲染通道
    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = swapChainFrameBuffers[imageIndex];
    renderPassBeginInfo.renderArea.offset = {0,0};
    renderPassBeginInfo.renderArea.extent = swapChainExtent;
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();

    // 开启渲染通道，所有的绘图命令都以vkCmd开头
    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    
    // 绑定渲染流水线
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    VkViewport viewport{};
    viewport.x = (swapChainExtent.width - VIEW_WIDTH) / 2;
    viewport.y = (swapChainExtent.height - VIEW_HEIGHT) / 2;
    viewport.width = VIEW_WIDTH;
    viewport.height = VIEW_HEIGHT;

    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // 绑定uniform descriptor
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    // 绘制三角形
    // vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(indices.size()),1,0,0,0);

    // 结束渲染通道
    vkCmdEndRenderPass(commandBuffer);

    // 结束commandbuffer
    if(vkEndCommandBuffer(commandBuffer)!=VK_SUCCESS)
        throw std::runtime_error("failed to record command buffer");
}