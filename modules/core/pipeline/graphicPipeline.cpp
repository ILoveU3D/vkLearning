#include <fstream>
#include <Application.h>

std::vector<char> readFile(const std::string& filename){
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if(!file.is_open())
        throw std::runtime_error("faild to open file" + filename);
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

// 将代码包装在shader module中
VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device){
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shaderModule;
    if(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule)!=VK_SUCCESS)
        throw std::runtime_error("faild to create shader module");
    return shaderModule;
}

void Application::createGraphicPipeline(){
    auto vertexShaderCode = readFile(VERTEX_SHADER_PATH);
    auto fragmentShaderCode = readFile(FRAGMENT_SHADER_PATH);

    auto vertShaderModule = createShaderModule(vertexShaderCode, device);
    auto fragmentShaderModule = createShaderModule(fragmentShaderCode, device);

    // 要使用着色器，还需要分配特定的管道阶段
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = vertShaderModule;
    vertexShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};

    // 保留一些流水线状态不在绘制中重新创建，例如视口，线宽等，使用动态状态保留这些属性
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

    // 创建顶点输入格式
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescription = Vertex::getAttributeDescriptions();
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
    vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescription.data();

    // 创建输入组件，并指定绘制的拓扑类型
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    // 创建视口与裁剪
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f; 
    VkRect2D scissor{};
    scissor.offset = {0,0};
    scissor.extent = swapChainExtent;
    VkPipelineViewportStateCreateInfo viewportCreateInfo{};
    viewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportCreateInfo.viewportCount = 1;
    viewportCreateInfo.pViewports = &viewport;
    viewportCreateInfo.scissorCount = 1;
    viewportCreateInfo.pScissors = &scissor;

    // 创建光栅化器
    VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo{};
    rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerCreateInfo.depthClampEnable = VK_FALSE;
    rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerCreateInfo.lineWidth = 1.0f;
    rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizerCreateInfo.depthBiasEnable = VK_FALSE;
    rasterizerCreateInfo.depthBiasClamp = 0.0f;
    rasterizerCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizerCreateInfo.depthBiasSlopeFactor = 0.0f;

    // 设置多重采样
    VkPipelineMultisampleStateCreateInfo multisampleCreateInfo{};
    multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleCreateInfo.minSampleShading = 1.0f;
    multisampleCreateInfo.pSampleMask = nullptr;
    multisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleCreateInfo.alphaToOneEnable = VK_FALSE;

    // 设置颜色混合
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo{};
    colorBlendCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendCreateInfo.attachmentCount = 1;
    colorBlendCreateInfo.pAttachments = &colorBlendAttachment;
    colorBlendCreateInfo.blendConstants[0] = 0.0f;
    colorBlendCreateInfo.blendConstants[1] = 0.0f;
    colorBlendCreateInfo.blendConstants[2] = 0.0f;
    colorBlendCreateInfo.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo{};
    depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilCreateInfo.depthTestEnable = VK_TRUE;
    depthStencilCreateInfo.depthWriteEnable = VK_TRUE;
    depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilCreateInfo.stencilTestEnable = VK_FALSE;

    // 统一着色器中的全局变量
    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges = 0;
    if(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout)!=VK_SUCCESS)
        throw std::runtime_error("failed to create pipeline layout");

    // 创建图形绘制流水线
    VkGraphicsPipelineCreateInfo graphicPipelineCreateInfo{};
    graphicPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicPipelineCreateInfo.stageCount = 2;
    graphicPipelineCreateInfo.pStages = shaderStages;
    graphicPipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    graphicPipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    graphicPipelineCreateInfo.pViewportState = &viewportCreateInfo;
    graphicPipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
    graphicPipelineCreateInfo.pMultisampleState = &multisampleCreateInfo;
    graphicPipelineCreateInfo.pColorBlendState = &colorBlendCreateInfo;
    graphicPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
    graphicPipelineCreateInfo.layout = pipelineLayout;
    graphicPipelineCreateInfo.renderPass = renderPass;
    graphicPipelineCreateInfo.subpass = 0;
    graphicPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    graphicPipelineCreateInfo.pDepthStencilState = &depthStencilCreateInfo;
    if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicPipelineCreateInfo, nullptr, &graphicPipeline)!=VK_SUCCESS)
        throw std::runtime_error("failed to create graphic pipeline");

    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}