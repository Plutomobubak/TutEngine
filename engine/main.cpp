#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/registry.h"
#include "util/ScriptCompiler.hpp"
#include "util/scene_loader.h"
#include "core/systems/renderSys.h"
#include "core/components/transform.h"
#include "core/systems/transformSys.h"
#include "core/systems/inputManager.h"

#include "renderer/renderer.h"
#include "renderer/shader.h"

#include "game/player.h"
#include "game/tutel.h"

#include "core/misc/debug.h"

// Window resize callback
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    float targetAspect = 4.0f / 3.0f;

    int vpWidth = width;
    int vpHeight = static_cast<int>(width / targetAspect);

    if (vpHeight > height) {
        vpHeight = height;
        vpWidth = static_cast<int>(height * targetAspect);
    }

    int xOffset = (width - vpWidth) / 2;
    int yOffset = (height - vpHeight) / 2;

    glViewport(xOffset, yOffset, vpWidth, vpHeight);
}
// Shader stuff
glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
glm::vec3 lightColor(1.0f);
float ambientStrength = 0.2f;
float specularStrength = 0.5f;
float shininess = 32.0f;

Debug debug;

void setupDebug() {
    debug.addVec3("Light Position", lightPos);
    debug.addVec3("Light Color", lightColor);
    debug.addFloat("Ambient Strength", ambientStrength, 0.f, 1.f);
    debug.addFloat("Specular Strength", specularStrength, 0.f, 1.f);
    debug.addFloat("Shininess", shininess, 1.f, 128.f);
}

int main (int argc, char *argv[]) {
   if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }


glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
int fbWidth, fbHeight;
glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
FramebufferSizeCallback(window, fbWidth, fbHeight);

//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    debug.init(window);
    setupDebug();

// Setup shaders
    Shader shader("shaders/basic.vert","shaders/basic.frag");
    shader.use();
    shader.setVec3("lightPos", lightPos);  // example light position
    shader.setVec3("lightColor", lightColor);            // white light
    shader.setFloat("ambientStrength", ambientStrength);
    shader.setFloat("specularStrength", specularStrength);
    shader.setFloat("shininess", shininess);
    // Setup ECS
    //
    ScriptCompiler::compileAllScripts("project/scripts","bin/scripts");
    auto& reg = Registry::instance();
    loadScene();

    std::cout << "scene loaded" << std::endl;

    reg.debugPrintComponents();

    RenderSys render(shader.getID());
    

    //reg.add<Parent>(Player::getEntity(),Parent{Tutel::getEntity()});


    InputManager::Init(window);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
 
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // delta T
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //Process input
        // Player::Update(deltaTime);
        // Tutel::Update(deltaTime);
        reg.updateAllComponents(deltaTime);


        // Update shader stuff
        shader.setVec3("lightPos", lightPos);  // example light position
        shader.setVec3("lightColor", lightColor);            // white light
        shader.setFloat("ambientStrength", ambientStrength);
        shader.setFloat("specularStrength", specularStrength);
        shader.setFloat("shininess", shininess);
        reg.view<Camera, GlobalTransform>([&](Entity e, Camera& cam, GlobalTransform& gt) {
            glm::vec3 camPos = glm::vec3(gt.matrix[3]);  // Extract position from matrix
            shader.setVec3("viewPos", camPos);
        });
        
        // Clear buffers
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

      debug.update();
      // Update states and render
      GlobalTransformSystem::update();
      InputManager::Update();
      render.render();

      // check and call events, swap the buffer
      glfwSwapBuffers(window);
    }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
