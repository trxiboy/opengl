#pragma once
#include <glm/glm.hpp>

struct Scene {
    float backgroundColor[3] = {1.0f, 1.0f, 1.0f};
};

struct Transform {
    float position[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale = 1.0f;
};

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::normalize(position - target);

    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;
    float sensitivity = 0.1f;
    bool firstMouse = true;
    float lastX = 400, lastY = 300;
    
    float FOV = 45.0f;
    float speed = 5.0f;
};