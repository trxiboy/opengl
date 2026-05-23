#include "overlay.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Overlay::Overlay() {
    
}
Overlay::~Overlay() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Overlay::initializeOverlay(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Overlay::loopStart() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

    void Overlay::drawUI(Scene& scene, Transform& transform, Camera& camera) {
        ImGui::Begin("Configuration (TAB) | Toggle mouse (C)");
        
        ImGui::Text("Object modifiers:");
        ImGui::SliderFloat3("color", scene.backgroundColor, 0.0f, 1.0f);
        ImGui::SliderFloat3("position", transform.position, -1.0f, 1.0f);
        ImGui::SliderFloat3("rotation", transform.rotation, 0.0f, 360.0f);
        ImGui::SliderFloat("scale", &transform.scale, 0.0f, 2.0f);
        ImGui::Text("Camera modifiers:");
        ImGui::SliderFloat("FOV", &camera.FOV, 45.0f, 120.0f);
        ImGui::SliderFloat("speed", &camera.speed, 1.0f, 20.0f);

        ImGui::End();
    }

void Overlay::loopEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}