#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "window.h"
#include "shader.h"
#include "overlay.h"

#include "types.h"
#include "camera.h"

#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.14159265358979323846

/* OpenGL Error Catching | Scrapped
#define GLCall(x) \
    x; \
    while (GLenum err = glGetError()) \
        std::cerr << "OpenGL error (" << #x << "): " << err << std::endl;
*/

constexpr int WIN_WIDTH = 1600;
constexpr int WIN_HEIGHT = 900;

constexpr GLenum polygonModes[] = { GL_FILL, GL_LINE, GL_POINT };

// Vertex data:
constexpr float cubeVertices[] = {
    // positions           // normals            // texcoords
    // Back face
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

    // Right face
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

    // Top face
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f
};
constexpr unsigned int cubeIndices[] = {
    0, 1, 2, 2, 3, 0,   // back
    4, 5, 6, 6, 7, 4,   // front
    8, 9,10,10,11, 8,   // left
    12,13,14,14,15,12,  // right
    16,17,18,18,19,16,  // bottom
    20,21,22,22,23,20   // top
};
GLsizei cubeIndiceCount = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

constexpr glm::vec3 cubeSpawnPositions[] = {
    glm::vec3(  3.2f,  -1.4f, -12.7f),
    glm::vec3( -5.1f,   2.8f,  -7.3f),
    glm::vec3(  1.7f,   4.5f, -15.9f),
    glm::vec3( -2.9f,  -3.2f,  -4.1f),
    glm::vec3(  6.4f,   1.1f, -10.5f),
    glm::vec3( -7.8f,   0.3f,  -8.2f),
    glm::vec3(  4.9f,  -2.7f,  -3.6f),
    glm::vec3( -3.3f,   5.0f, -14.2f),
    glm::vec3(  2.1f,  -4.4f,  -6.8f),
    glm::vec3( -1.6f,   3.7f,  -9.9f),

    // glm::vec3(  8.2f,  -1.1f, -11.3f),
    // glm::vec3( -6.5f,   2.2f,  -5.4f),
    // glm::vec3(  0.9f,  -3.8f,  -2.7f),
    // glm::vec3(  5.7f,   4.1f, -13.6f),
    // glm::vec3( -4.4f,  -0.9f,  -7.7f),
    // glm::vec3(  3.8f,   3.3f,  -4.9f),
    // glm::vec3( -2.2f,  -4.6f, -10.1f),
    // glm::vec3(  7.1f,   1.9f,  -9.4f),
    // glm::vec3( -8.7f,   0.6f, -12.0f),
    // glm::vec3(  1.4f,   5.3f,  -3.3f),

    // glm::vec3( -3.9f,  -2.5f, -14.8f),
    // glm::vec3(  6.0f,   2.7f,  -6.1f),
    // glm::vec3( -1.1f,   4.9f,  -8.4f),
    // glm::vec3(  4.2f,  -3.1f, -11.7f),
    // glm::vec3( -5.6f,   1.4f,  -3.8f),
    // glm::vec3(  2.6f,  -4.8f,  -9.2f),
    // glm::vec3( -7.3f,   3.5f, -13.1f),
    // glm::vec3(  0.5f,   2.0f,  -5.6f),
    // glm::vec3(  8.9f,  -0.7f, -10.9f),
    // glm::vec3( -4.8f,   4.4f,  -7.0f)
};

constexpr unsigned int cubeSpawnCount = sizeof(cubeSpawnPositions) / sizeof(cubeSpawnPositions[0]);

// Modifiers:
Scene scene;
Camera camera;

// Object vector:
std::vector<Object> objects;

void spawnCubes(unsigned int texture) {
    objects.reserve(cubeSpawnCount);

    for (unsigned int i = 0; i < cubeSpawnCount; i++) {
        Object obj;
        obj.transform.position = cubeSpawnPositions[i];
        // obj.material.color = { 0.5f, 0.5f, 0.5f }
        obj.material.textureID = texture;
        obj.material.useTexture = false;

        objects.push_back(obj);
    }
}

struct CubeBuffers {
    GLuint objectVAO = 0;
    GLuint lightVAO  = 0;
};

CubeBuffers setupBuffers() {
    CubeBuffers buffers;
    GLuint VBO, EBO;

    glGenVertexArrays(1, &buffers.objectVAO);
    glBindVertexArray(buffers.objectVAO);

    // VBO (positions + normals + texcoords)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // layout (location = 0) in vec3 aPos;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout (location = 1) in vec3 aNormal;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // layout (location = 2) in vec2 aTexCoord;
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &buffers.lightVAO);
    glBindVertexArray(buffers.lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glPointSize(3.0f);

    return buffers;
}

unsigned int loadTexture(const char* path) {
    int width, height, nrChannels;
    unsigned int texture;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

void render(Shader& lightingShader, Shader& lightCubeShader, const CubeBuffers& buffers, float aspect) {
    glPolygonMode(GL_FRONT_AND_BACK, polygonModes[scene.polygonMode]);
    glClearColor(scene.backgroundColor[0], scene.backgroundColor[1], scene.backgroundColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = camera.getViewMatrix();
    const glm::mat4 projection = camera.getProjectionMatrix(aspect);

    lightingShader.use();
    glBindVertexArray(buffers.objectVAO);

    lightingShader.setMat4("view", view);
    lightingShader.setMat4("projection", projection);
    lightingShader.setInt("ourTexture", 0);
    lightingShader.setVec3("lightColor", scene.light.color);
    lightingShader.setVec3("lightPos", scene.light.position);

    for (const Object& obj : objects) {
        if (!obj.visible) continue;

        const Transform& t = obj.transform;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, t.position);                                   // position XYZ
        model = glm::rotate(model, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));  // pitch |
        model = glm::rotate(model, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));  // yaw   | Rotation
        model = glm::rotate(model, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));  // roll  |
        model = glm::scale(model, t.scale);   // size

        lightingShader.setMat4("model", model);
        lightingShader.setVec3("objectColor", obj.material.color);
        lightingShader.setBool("useTexture", obj.material.useTexture);

        if (obj.material.useTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj.material.textureID);
        }

        glDrawElements(GL_TRIANGLES, cubeIndiceCount, GL_UNSIGNED_INT, 0);
    }

    if (!scene.light.visible) return;

    lightCubeShader.use();
    glBindVertexArray(buffers.lightVAO);

    lightCubeShader.setMat4("view", view);
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setVec3("lightColor", scene.light.color);

    glm::mat4 lampModel = glm::mat4(1.0f);
    lampModel = glm::translate(lampModel, scene.light.position);
    lampModel = glm::scale(lampModel, glm::vec3(0.2f));
    lightCubeShader.setMat4("model", lampModel);

    glDrawElements(GL_TRIANGLES, cubeIndiceCount, GL_UNSIGNED_INT, 0);
}

int main() {
    Window window(WIN_WIDTH, WIN_HEIGHT, &camera);
    if (!window.initialize()) return -1;

    Shader lightingShader("shaders/cube.vs", "shaders/cube.fs");
    Shader lightCubeShader("shaders/lighting.vs", "shaders/lighting.fs");

    CubeBuffers buffers = setupBuffers();

    unsigned int texture = loadTexture("content/texture.jpg");
    spawnCubes(texture);

    Overlay overlay(window.getGLFWwindow());

    while (!glfwWindowShouldClose(window.getGLFWwindow())) {
        window.processInput(camera);

        render(lightingShader, lightCubeShader, buffers, window.getAspect());

        overlay.loopStart();
        overlay.drawUI(scene, objects, camera);
        overlay.loopEnd();

        window.refresh();
    }
    return 0;
}
