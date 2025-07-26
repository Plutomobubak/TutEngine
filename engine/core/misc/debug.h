#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Debug {
public:
    void init(GLFWwindow* window);
    void update();

    // Register parameters to be tweaked in UI
    void addFloat(const std::string& name, float& value, float min, float max);
    void addVec3(const std::string& name, glm::vec3& value);
    void addBool(const std::string& name, bool& value);
    // add more types as needed...

private:
    struct DebugParam {
        enum class Type { Float, Vec3, Bool } type;
        void* ptr;  // raw pointer to the parameter variable
        float min, max;  // for floats
        std::string name;
    };
    std::vector<DebugParam> params;
};
