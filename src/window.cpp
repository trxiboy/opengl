#include "window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool imguiFocus = false;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (imguiFocus)
        return;

    Camera& camera = *(Camera*)glfwGetWindowUserPointer(window);

    if (camera.firstMouse) {
        camera.lastX = xpos;
        camera.lastY = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastX;
    float yoffset = camera.lastY - ypos;
    camera.lastX = xpos;
    camera.lastY = ypos;

    xoffset *= camera.sensitivity;
    yoffset *= camera.sensitivity;

    camera.yaw   += xoffset;
    camera.pitch += yoffset;

    camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(direction);
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

                camPtr->firstMouse = true;

                double cx = width / 2.0;
                double cy = height / 2.0;
                glfwSetCursorPos(window, cx, cy);
                camPtr->lastX = cx;
                camPtr->lastY = cy;
            }
            tabPressed = true;
        }
    } else {
        tabPressed = false;
    }

    if (imguiFocus)
        return;

    const float cameraSpeed = camera.speed * deltaTime;

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(camera.front, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, camera.front));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += cameraSpeed * camera.front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= cameraSpeed * camera.front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= right * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += right * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.position += up * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.position -= up * cameraSpeed;
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
    double cx = width / 2.0;
    double cy = height / 2.0;

    glfwSetCursorPos(window, cx, cy);

    camPtr->firstMouse = true;
    camPtr->lastX = cx;
    camPtr->lastY = cy;
}

void Window::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}