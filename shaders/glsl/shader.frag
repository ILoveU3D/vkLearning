#version 450 core

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 fragPosition;
layout(location = 4) in vec3 viewPosition;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

// 环境光强
float ambientStrength = 0.2;

// 漫反射光源
vec3 lightPostion = vec3(3.0);

// 镜面光强
float specularStrength = 0.5;

void main() {
    // 环境光
    vec3 ambientLight = ambientStrength * vec3(1.0);

    // 漫反射光
    vec3 norm  = normalize(normal);
    vec3 lightDirection = normalize(lightPostion - fragPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // 镜面光
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float shininess = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * shininess * vec3(1.0);

    vec4 objectTexutre = texture(texSampler, fragTexCoord);
    outColor = vec4(ambientLight + diffuse + specular, 1.0) * objectTexutre;
}