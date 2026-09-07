#pragma once
#include <glm/glm.hpp>

struct Light {
    glm::vec3 position = glm::vec3(1.2f, 1.0f, 2.0f);
    glm::vec3 color    = glm::vec3(1.0f);
    bool visible       = true;
};

struct Scene {
    float backgroundColor[3] = {0.05f, 0.05f, 0.08f};
    Light light;
    int polygonMode = 0;
};

struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);   // euler
    glm::vec3 scale    = glm::vec3(1.0f);
};

struct Material {
    glm::vec3 color = glm::vec3(1.0f);
    bool useTexture = false;
    unsigned int textureID = 0;
};

struct Object {
    Transform transform;
    Material material;
    bool visible = true;
};
