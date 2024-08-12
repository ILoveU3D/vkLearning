#include <unordered_map>

#include <Application.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// 定义哈希函数用于unordered_map实现
namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                   (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

void Application::loadModel(){
    tinyobj::attrib_t attribution;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    if(!tinyobj::LoadObj(&attribution, &shapes, &materials, &warning, &error, MODEL_PATH.c_str()))
        throw std::runtime_error(warning + error);

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for(const auto& shape:shapes){
        for(const auto& index:shape.mesh.indices){
            Vertex vertex{};
            vertex.pos = {
                attribution.vertices[3*index.vertex_index + 0],
                attribution.vertices[3*index.vertex_index + 1],
                attribution.vertices[3*index.vertex_index + 2]
            };
            vertex.texCoord = {
                attribution.texcoords[2 * index.texcoord_index + 0],
                1.0f - attribution.texcoords[2 * index.texcoord_index + 1],
            };
            vertex.color = {1.0f, 1.0f, 1.0f};
            if(uniqueVertices.count(vertex) == 0){
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}