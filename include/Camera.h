#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement{
    FORWARD, BACK, LEFT, RIGHT
};

class Camera{
public:
    // 相机自身属性（可变）
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    // 相机设置（可变）
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera():position(glm::vec3(0.0f,0.0f,0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),front(glm::vec3(0.0f,0.0f,-1.0f)), yaw(-90), pitch(0), movementSpeed(2.5f), mouseSensitivity(0.2f), zoom(45){
        worldUp = up;
        updateCamera();
    }

    glm::mat4 getView(){
        return glm::lookAt(position, position+front, up);
    }

    // 几个响应函数
    void responseKey(CameraMovement direction, float currentDeltaFrame){
        float velocity = movementSpeed * currentDeltaFrame;
        switch (direction){
        case FORWARD:
            position += front * velocity;
            break;
        case BACK:
            position -= front * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
        }
    }

    void responseMouseMovement(float offsetX, float offsetY){
        offsetX *= mouseSensitivity;
        offsetY *= mouseSensitivity;
        yaw += offsetX;
        pitch += offsetY;
        pitch = std::clamp(pitch, 89.0f, -89.0f);
        updateCamera();
    }

    void responseMouseScroll(float offset){
        zoom -= offset;
        zoom = std::clamp(zoom, 1.0f, 45.0f);
    }


private:
    void updateCamera(){
        // 计算新方向
        glm::vec3 front = glm::vec3(cos(glm::radians(yaw))*cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw))*cos(glm::radians(pitch)));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};