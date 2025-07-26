#include "debug.h"

void Debug::init(GLFWwindow* window){
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

int windowWidth, windowHeight;
glfwGetWindowSize(window, &windowWidth, &windowHeight);

ImGuiIO& io = ImGui::GetIO();
io.DisplaySize = ImVec2((float)windowWidth, (float)windowHeight);

// Also get framebuffer size for retina/HiDPI
int fbWidth, fbHeight;
glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
io.DisplayFramebufferScale = ImVec2(
    fbWidth > 0 ? ((float)fbWidth / windowWidth) : 1.f,
    fbHeight > 0 ? ((float)fbHeight / windowHeight) : 1.f);

  io.FontGlobalScale = 1.0f;printf("Display Size: %.1f x %.1f\n", io.DisplaySize.x, io.DisplaySize.y);
printf("Framebuffer Scale: %.2f x %.2f\n", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core"); 
}

void Debug::update() {
      ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    ImGui::Begin("Debug Controls");
ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetWindowSize(ImVec2(400, 300), ImGuiCond_Once);

    for (auto& param : params) {
        switch (param.type) {
            case DebugParam::Type::Float:
                ImGui::SliderFloat(param.name.c_str(), static_cast<float*>(param.ptr), param.min, param.max);
                break;
            case DebugParam::Type::Vec3:
ImGui::ColorEdit3(param.name.c_str(), glm::value_ptr(*static_cast<glm::vec3*>(param.ptr)));
                break;
            case DebugParam::Type::Bool:
                ImGui::Checkbox(param.name.c_str(), static_cast<bool*>(param.ptr));
                break;
        }
    }

    ImGui::End();
ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Debug::addFloat(const std::string& name, float& value, float min, float max) {
    params.push_back({ DebugParam::Type::Float, &value, min, max, name });
}

void Debug::addVec3(const std::string& name, glm::vec3& value) {
    params.push_back({ DebugParam::Type::Vec3, &value, 0, 0, name });
}

void Debug::addBool(const std::string& name, bool& value) {
    params.push_back({ DebugParam::Type::Bool, &value, 0, 0, name });
}
