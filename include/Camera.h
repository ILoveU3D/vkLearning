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

    Camera();
    // 部分接口
    glm::mat4 getViewMatrix();
    // 几个响应函数
    void responseKey(CameraMovement direction, float currentDeltaFrame);
    void responseMouseMovement(float positionX, float positionY);
    void responseMouseScroll(float offset);

private:
    glm::vec2 lastPosition;
    bool initPosition;
    void updateCamera();
};