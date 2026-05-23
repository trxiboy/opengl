#pragma once
#include <GLFW/glfw3.h>
#include "types.h"

class Overlay {
public:
    Overlay();
    ~Overlay();

    void initializeOverlay(GLFWwindow* window);
    void loopStart();
    void drawUI(Scene& scene, Transform& transform, Camera& camera);
    void loopEnd();
};