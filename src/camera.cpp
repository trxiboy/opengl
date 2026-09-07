#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

Camera::Camera() {
    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    const float velocity = speed * deltaTime;

    switch (direction) {
        case CameraMovement::Forward:
            position += front * velocity;
            break;
        case CameraMovement::Backward:
            position -= front * velocity; 
            break;
        case CameraMovement::Left:
            position -= right * velocity;
            break;
        case CameraMovement::Right:
            position += right * velocity;
            break;
        case CameraMovement::Up:
            position += up * velocity;
            break;
        case CameraMovement::Down:
            position -= up * velocity;
            break;
    }
}

void Camera::processMouseMovement(float xpos, float ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    yaw   += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    updateVectors();
}

void Camera::processScroll(float yoffset) {
    FOV -= yoffset * zoomStep;
    FOV = glm::clamp(FOV, minFOV, maxFOV);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(FOV), aspect, nearPlane, farPlane);
}

void Camera::resetMouse(float centerX, float centerY) {
    firstMouse = true;
    lastX = centerX;
    lastY = centerY;
}
