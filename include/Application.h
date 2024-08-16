#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Vertex.h>

const std::string VERTEX_SHADER_PATH = std::string(SHADER_PATH) + "vert.spv";
const std::string FRAGMENT_SHADER_PATH = std::string(SHADER_PATH) + "frag.spv";
const std::string MODEL_PATH = std::string(ASSET_PATH) + "viking_room.obj";
const std::string TEXTURE_PATH = std::string(ASSET_PATH) + "viking_room.png";
const int WINDOW_WIDTH = 2560;
const int WINDOW_HEIGHT = 1440;

class Application{
public:
    void run();
    // 动态调整窗口
    bool framebufferResized = false;
private:
    // 记录全局帧
    uint32_t currentFrame = 0;
    // glfw窗体
    GLFWwindow* window;
    // Vulkan实例
    VkInstance instance;
    // 是否启用验证层
#ifdef NDBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif
    // 验证层扩展
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    // 回调信使
    VkDebugUtilsMessengerEXT debugMessenger;
    // 物理设备
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    // 逻辑设备
    VkDevice device;
    // graphic队列
    VkQueue graphicQueue;
    // 窗口表面
    VkSurfaceKHR surface;
    // present队列
    VkQueue presentQueue;
    // swap chain设备扩展
    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    // swap chain
    VkSwapchainKHR swapChain;
    // swap chain中的图像
    std::vector<VkImage> swapChainImages;
    // 临时存储，swapchain的图像显示格式
    VkFormat swapChainImageFormat;
    // extent大小
    const uint32_t VIEW_WIDTH = 800;
    const uint32_t VIEW_HEIGHT = 800;
    // 临时存储，swapchain的交换范围
    VkExtent2D swapChainExtent;
    // 记录交换链中的图像视图
    std::vector<VkImageView> swapChainImageViews;
    // 渲染通道
    VkRenderPass renderPass;
    // shader uniform变量绑定，layout用于描述可以绑定的描述符
    VkDescriptorSetLayout descriptorSetLayout;
    // 描述符不能直接创建，需要创建描述符池来分配
    VkDescriptorPool descriptorPool;
    // 分配的描述符集
    std::vector<VkDescriptorSet> descriptorSets;
    // 统一shader中的状态变量
    VkPipelineLayout pipelineLayout;
    // 渲染流水线
    VkPipeline graphicPipeline;
    // 帧缓冲区
    std::vector<VkFramebuffer> swapChainFrameBuffers;
    // 命令池
    VkCommandPool commandPool;
    // 一个小的优化：允许多帧在显示过程的后台渲染
    const size_t MAX_FRAMES_IN_FLIGHT = 2;
    // 命令缓冲区
    std::vector<VkCommandBuffer> commandBuffers;
    // 一些semaphore与fence用于控制swapchain，renderpass和帧与帧之间的同步
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishSemaphores;
    std::vector<VkFence> inFlightFences;
    // 顶点与索引（从加载）
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    // 顶点缓冲区
    VkBuffer vertexBuffer;
    // 为顶点缓冲区分配的内存
    VkDeviceMemory vertexBufferMemory;
    // 顶点索引缓冲区
    VkBuffer indexBuffer;
    // 为顶点索引缓冲区分配的内存
    VkDeviceMemory indexBufferMemory;
    // descriptor的缓冲
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    // mipmap节约贴图开销
    uint32_t mipLevels;
    // 纹理贴图，贴图缓存与纹理图像视图
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    // 贴图采样器
    VkSampler textureSampler;
    // 绘制3D图形时需要载入深度图
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    void initWindow();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void initVulkan();
    void mainLoop();
    void cleanUp();

    void createInstance();
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSurface();
    void createSwapChain();
    void createImageViews();
    void createGraphicPipeline();
    void createRenderPass();
    void createFramebuffers();
    void createCommandPool();
    void createCommandBuffer();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void drawFrame();
    void createSyncObjects();
    void cleanupSwapChain();
    void recreateSwapChain();
    void createBuffer(VkDeviceSize deviceSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createVertexBuffer();
    void createIndexBuffer();
    void createDescriptorSetLayout();
    void createUniformBuffers();
    void updateUniformBuffer();
    void createDescriptorPool();
    void createDescriptorSet();
    void createTextureImage();
    void createImage(uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    VkCommandBuffer beginSingleCommand();
    void endSingleCommand(VkCommandBuffer commandBuffer);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, uint32_t mipLevels);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createTextureImageView();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    void createTextureSampler();
    void createDepthResources();
    void loadModel();
    void generateMipmaps(VkImage image, uint32_t texWidth, uint32_t texHeight, uint32_t mipLevels);
};