#pragma once

#include <glm/glm.hpp>

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class Camera {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right    = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 worldUp  = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw   = -90.0f;
    float pitch = 0.0f;

    float FOV         = 45.0f;
    float speed       = 5.0f;
    float sensitivity = 0.1f;
    float zoomStep    = 2.0f;

    static constexpr float minFOV    = 1.0f;
    static constexpr float maxFOV    = 120.0f;
    static constexpr float nearPlane = 0.1f;
    static constexpr float farPlane  = 100.0f;

    Camera();

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xpos, float ypos);
    void processScroll(float yoffset);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;

    void resetMouse(float centerX, float centerY);

private:
    void updateVectors();

    bool  firstMouse = true;
    float lastX = 0.0f;
    float lastY = 0.0f;
};
