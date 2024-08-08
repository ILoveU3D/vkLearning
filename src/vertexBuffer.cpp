#include <HelloTriangleApplication.h>

void HelloTriangleApplication::createVertexBuffer(){
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    VkBuffer stageBuffer;
    VkDeviceMemory stageBufferMemory;

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stageBuffer, stageBufferMemory);

    // 复制顶点信息到缓冲区
    void* data;
    vkMapMemory(device, stageBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stageBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stageBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stageBuffer, nullptr);
    vkFreeMemory(device, stageBufferMemory, nullptr);
}

void HelloTriangleApplication::createIndexBuffer(){
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    VkBuffer stageBuffer;
    VkDeviceMemory stageBufferMemory;

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stageBuffer, stageBufferMemory);

    void* data;
    vkMapMemory(device, stageBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stageBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stageBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(device, stageBuffer, nullptr);
    vkFreeMemory(device, stageBufferMemory, nullptr);
}