#include <vector>

// 需要检查交换链的三种属性：基本表面功能（图像容量，图像宽高），表面格式，演示模式
struct SwapChainSupport{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};