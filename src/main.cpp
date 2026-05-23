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

#include <iostream>
#include <cmath>

#define PI 3.14159265358979323846

/* OpenGL Error Catching
#define GLCall(x) \
    x; \
    while (GLenum err = glGetError()) \
        std::cerr << "OpenGL error (" << #x << "): " << err << std::endl;
*/

constexpr int WIN_WIDTH = 800;
constexpr int WIN_HEIGHT = 600;

// Vertex data:
constexpr float cubeVertices[] = {
    // positions           // texcoords
    // Back face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

    // Left face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

    // Right face
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

    // Top face
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f
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

glm::vec3 cubePositions[] = {
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

    glm::vec3(  8.2f,  -1.1f, -11.3f),
    glm::vec3( -6.5f,   2.2f,  -5.4f),
    glm::vec3(  0.9f,  -3.8f,  -2.7f),
    glm::vec3(  5.7f,   4.1f, -13.6f),
    glm::vec3( -4.4f,  -0.9f,  -7.7f),
    glm::vec3(  3.8f,   3.3f,  -4.9f),
    glm::vec3( -2.2f,  -4.6f, -10.1f),
    glm::vec3(  7.1f,   1.9f,  -9.4f),
    glm::vec3( -8.7f,   0.6f, -12.0f),
    glm::vec3(  1.4f,   5.3f,  -3.3f),

    glm::vec3( -3.9f,  -2.5f, -14.8f),
    glm::vec3(  6.0f,   2.7f,  -6.1f),
    glm::vec3( -1.1f,   4.9f,  -8.4f),
    glm::vec3(  4.2f,  -3.1f, -11.7f),
    glm::vec3( -5.6f,   1.4f,  -3.8f),
    glm::vec3(  2.6f,  -4.8f,  -9.2f),
    glm::vec3( -7.3f,   3.5f, -13.1f),
    glm::vec3(  0.5f,   2.0f,  -5.6f),
    glm::vec3(  8.9f,  -0.7f, -10.9f),
    glm::vec3( -4.8f,   4.4f,  -7.0f)
};

unsigned int cubeCount = sizeof(cubePositions) / sizeof(cubePositions[0]);

// Modifiers:
Scene scene;
Transform transform;
Camera camera;

GLuint setupBuffers() {
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO (positions + texcoords)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // layout (location = 0) in vec3 aPos;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout (location = 2) in vec2 aTexCoord;
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return VAO;
}

unsigned int loadTexture() {
    int width, height, nrChannels;
    unsigned int texture;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("content/texture2.jpg", &width, &height, &nrChannels, 0);

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
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

void render(Shader& shader, GLuint VAO, float time) {
    glClearColor(scene.backgroundColor[0], scene.backgroundColor[1], scene.backgroundColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    for(unsigned int i = 0; i < cubeCount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(transform.position[0], transform.position[1], transform.position[2])+cubePositions[i]); // position XYZ
        model = glm::rotate(model, glm::radians(transform.rotation[0]*(i+1)*time), glm::vec3(1, 0, 0));       // pitch |
        model = glm::rotate(model, glm::radians(transform.rotation[1]*(i+1)*time), glm::vec3(0, 1, 0));       // yaw   | Rotation
        model = glm::rotate(model, glm::radians(transform.rotation[2]*(i+1)*time), glm::vec3(0, 0, 1));       // roll  |
        model = glm::scale(model, glm::vec3(transform.scale, transform.scale, transform.scale));              // size
        
        glm::mat4 view;
        view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.FOV), 800.0f / 600.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, cubeIndiceCount, GL_UNSIGNED_INT, 0);
    }
}

int main() {
    Window window(WIN_WIDTH, WIN_HEIGHT, &camera);
    if (!window.initialize()) return -1;

    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    GLuint VAO = setupBuffers();

    // unsigned int texture = loadTexture();

    Overlay overlay;
    overlay.initializeOverlay(window.getGLFWwindow());

    while (!glfwWindowShouldClose(window.getGLFWwindow())) {
        window.processInput(camera);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        render(shader, VAO, static_cast<float>(glfwGetTime()));

        overlay.loopStart();
        overlay.drawUI(scene, transform, camera);
        overlay.loopEnd();

        window.refresh();
    }
    return 0;
}
