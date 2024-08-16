#include <algorithm>

#include <Camera.h>

Camera::Camera():position(glm::vec3(0.0f,-5.0f,0.0f)), up(glm::vec3(0.0f, 0.0f, 1.0f)),front(glm::vec3(0.0f,1.0f,0.0f)), yaw(0.0f), pitch(0.0f), movementSpeed(4.0f), mouseSensitivity(0.02f), zoom(45), initPosition(true){
    worldUp = up;
    updateCamera();
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(position, position+front, up);
}

void Camera::responseKey(CameraMovement direction, float currentDeltaFrame){
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

void Camera::responseMouseMovement(float positionX, float positionY){
    if(!initPosition){
        glm::vec2 offset = glm::vec2(positionX - lastPosition.x, positionY -  lastPosition.y);
        offset *= mouseSensitivity;
        yaw += offset.x;
        pitch += offset.y;
        pitch = std::clamp(pitch, -89.0f, 89.0f);
        updateCamera();
    }else
        initPosition = false;
    lastPosition = glm::vec2(positionX, positionY);
}

void Camera::responseMouseScroll(float offset){
    zoom -= offset;
    zoom = std::clamp(zoom, 1.0f, 45.0f);
}

void Camera::updateCamera(){
    // 计算新方向
    front = glm::vec3(glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)), glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)), -glm::sin(glm::radians(pitch)));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}