#pragma once
#include <GLFW/glfw3.h>
#include "types.h"
#include "camera.h"

#include <vector>

class Overlay {
public:
    Overlay(GLFWwindow* window);
    ~Overlay();

    void loopStart();
    void drawUI(Scene& scene, std::vector<Object>& objects, Camera& camera);
    void loopEnd();

private:
    int selected = 0;
};
