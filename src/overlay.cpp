#include "overlay.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>

#include <cstdio>

Overlay::Overlay(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}
Overlay::~Overlay() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Overlay::loopStart() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Overlay::drawUI(Scene& scene, std::vector<Object>& objects, Camera& camera) {
    ImGui::Begin("Configuration (TAB) | Toggle mouse (C)");

    ImGui::Text("Scene:");
    ImGui::DragFloat3("Light position", glm::value_ptr(scene.light.position), 0.1f);
    ImGui::ColorEdit3("Light color", glm::value_ptr(scene.light.color));
    ImGui::ColorEdit3("background", scene.backgroundColor);
    ImGui::Combo("Polygon mode", &scene.polygonMode, "Fill\0Wireframe\0Point\0");

    ImGui::Separator();

    if (objects.empty()) {
        ImGui::Text("No objects in scene.");
        ImGui::End();
        return;
    }

    if (selected >= (int)objects.size()) selected = (int)objects.size() - 1;
    if (selected < 0) selected = 0;

    char label[32];
    snprintf(label, sizeof(label), "Cube %d", selected);

    ImGui::Text("Object modifiers:");
    if (ImGui::BeginCombo("object", label)) {
        for (int i = 0; i < (int)objects.size(); i++) {
            char item[32];
            snprintf(item, sizeof(item), "Cube %d", i);
            if (ImGui::Selectable(item, selected == i))
                selected = i;
            if (selected == i)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    Object& o = objects[selected];

    ImGui::Checkbox("visible", &o.visible);
    ImGui::ColorEdit3("color", glm::value_ptr(o.material.color));
    ImGui::Checkbox("use texture", &o.material.useTexture);
    ImGui::DragFloat3("position", glm::value_ptr(o.transform.position), 0.1f);
    ImGui::SliderFloat3("rotation", glm::value_ptr(o.transform.rotation), 0.0f, 360.0f);
    ImGui::DragFloat3("scale", glm::value_ptr(o.transform.scale), 0.1f);

    if (ImGui::Button("Apply material to all")) {
        for (Object& other : objects)
            other.material = o.material;
    }

    ImGui::Separator();

    ImGui::Text("Camera modifiers:");
    ImGui::SliderFloat("FOV", &camera.FOV, Camera::minFOV, Camera::maxFOV);
    ImGui::SliderFloat("speed", &camera.speed, 1.0f, 20.0f);
    ImGui::SliderFloat("sensitivity", &camera.sensitivity, 0.01f, 1.0f);

    ImGui::End();
}

void Overlay::loopEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
