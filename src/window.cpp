#include "window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool imguiFocus = false;

static Camera& cameraFor(GLFWwindow* window) {
    return *(Camera*)glfwGetWindowUserPointer(window);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (imguiFocus)
        return;

    cameraFor(window).processMouseMovement((float)xpos, (float)ypos);
}

void scrollCallback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
    if (imguiFocus)
        return;

    cameraFor(window).processScroll((float)yoffset);
}

Window::Window(unsigned int width, unsigned int height, Camera* cam)
    : width(width), height(height), camPtr(cam) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::initialize() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }

    glfwMakeContextCurrent(window);
    
    glfwSetWindowUserPointer(window, camPtr);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, width, height);
    return true;
}

float Window::getAspect() const {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    if (h == 0) return 1.0f;   // minimized
    return (float)w / (float)h;
}

void Window::processInput(Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool f11Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
        if (!f11Pressed) {
            toggleFullscreen();
            f11Pressed = true;
        }
    } else {
        f11Pressed = false;
    }

    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!tabPressed) {
            imguiFocus = !imguiFocus;
            if (imguiFocus) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                centerCursor();
            }
            tabPressed = true;
        }
    } else {
        tabPressed = false;
    }

    if (imguiFocus)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Forward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Backward, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Left, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Right, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Up, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::Down, deltaTime);
}

void Window::refresh() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::toggleFullscreen() {
    isFullscreen = !isFullscreen;

    if (isFullscreen) {
        glfwGetWindowPos(window, &windowedX, &windowedY);
        glfwGetWindowSize(window, &windowedW, &windowedH);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(
            window,
            monitor,
            0, 0,
            mode->width,
            mode->height,
            mode->refreshRate
        );
    } else {
        glfwSetWindowMonitor(
            window,
            nullptr,
            windowedX,
            windowedY,
            windowedW,
            windowedH,
            0
        );
    }
    centerCursor();
}

void Window::centerCursor() {
    int w, h;
    glfwGetWindowSize(window, &w, &h);

    double cx = w / 2.0;
    double cy = h / 2.0;

    glfwSetCursorPos(window, cx, cy);
    camPtr->resetMouse((float)cx, (float)cy);
}

void Window::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}