#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "types.h"


class Window {
public:
    Window(unsigned int width, unsigned int height, Camera* cam);
    ~Window();

    bool initialize();
    void processInput(Camera& camera);
    void refresh();
    void toggleFullscreen();
    
    GLFWwindow* getGLFWwindow() const { return window; }

    Camera* camPtr;

    float deltaTime, lastFrame;
private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    unsigned int width;
    unsigned int height;

    bool isFullscreen = false;
    int windowedX = 0, windowedY = 0;
    int windowedW = 0, windowedH = 0;

    GLFWmonitor* monitor = nullptr;
    GLFWwindow* window = nullptr;
};
