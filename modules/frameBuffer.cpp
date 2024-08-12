#include <array>

#include <Application.h>

void Application::createFramebuffers(){
    swapChainFrameBuffers.resize(swapChainImageViews.size());

    // 遍历视图，创建帧缓冲区
    for(size_t i = 0;i < swapChainImageViews.size();i++){
        std::array<VkImageView, 2> attachments = {swapChainImageViews[i], depthImageView};
        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = swapChainExtent.width;
        framebufferCreateInfo.height = swapChainExtent.height;
        framebufferCreateInfo.layers = 1;
        if(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &swapChainFrameBuffers[i])!=VK_SUCCESS)
            throw std::runtime_error("failed to create framebuffer");
    }
}