#include <HelloTriangleApplication.h>

void HelloTriangleApplication::initVulkan(){
    // 创建用例
    createInstance();
    // 设置回调信使
    setupDebugMessenger();
    // 创建窗体surface
    createSurface();
    // 选择物理设备
    pickPhysicalDevice();
    // 创建逻辑设备
    createLogicalDevice();
    // 创建交换链
    createSwapChain();
    // 创建交换链中每个图像的图像视图
    createImageViews();
    // 在创建绘制流水线前，需要设定渲染使用的framebuffer组件，指定颜色与缓冲区，以及样本数量
    createRenderPass();
    // 创建shader全局变量的描述符
    createDescriptorSetLayout();
    // 创建图形绘制流水线，图形绘制流水线包括以下过程：
    // TA
    // 1.输入组装（input assembler）
    // 2.顶点着色器（vertex shader）,将空间坐标系转化为屏幕坐标系
    // 3.曲面细分（tessellation），细分几何形体
    // 4.几何着色器（geometry shader），细分几何形体
    // 3D
    // 5.光栅化（rasterization），将图元离散化为片段
    // 6.片段着色器（fragment shader），片段写入framebuffer，并决定颜色与深度
    // 7.颜色混合（blending），将framebuffer进行混合或透明度相加
    // 输出framebuffer
    createGraphicPipeline();
    // 创建命令池
    createCommandPool();
    // 加载深度资源
    createDepthResources();
    // 创建framebuffer
    createFramebuffers();
    // 加载贴图
    createTextureImage();
    // 加载贴图视图
    createTextureImageView();
    // 创建取样器
    createTextureSampler();
    // 加载模型
    loadModel();
    // 创建顶点缓冲区，用于暂存顶点数据
    createVertexBuffer();
    // 创建索引缓冲区
    createIndexBuffer();
    // 创建uniform变量缓冲区
    createUniformBuffers();
    // 创建descriptor池
    createDescriptorPool();
    // 创建descriptor set
    createDescriptorSet();
    // 分配单个命令池的命令缓冲区
    createCommandBuffer();
    // 创建信号量
    createSyncObjects();
}