#include <optional>

struct QueueFamilyIndice {
    std::optional<uint32_t> queueFamily;
    
    bool isOK(){
        return queueFamily.has_value();
    }
};

struct QueueFamilyIndice2
{
    std::optional<uint32_t> graphicFamily;
    std::optional<uint32_t> presentFamily;

    bool isOK(){
        return graphicFamily.has_value() && presentFamily.has_value();
    }
};
