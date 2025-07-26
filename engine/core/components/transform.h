#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../component.h"

using Entity = unsigned int;
struct Transform : public Component {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f}; // Euler angles
    glm::vec3 scale{1.0f};

    Transform(const glm::vec3& pos = glm::vec3(0), 
              const glm::vec3& rot = glm::vec3(0), 
              const glm::vec3& scl = glm::vec3(1))
        : position(pos), rotation(rot), scale(scl) {}

    glm::mat4 getLocalMatrix() const {
        glm::mat4 mat = glm::translate(glm::mat4(1.0f), position);
        mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));  // yaw
        mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));  // pitch
        mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));  // roll
        mat = glm::scale(mat, scale);
        return mat;
    }
    void applyProperties(const Properties& props) {
        for (const auto& [key, val] : props) {
            if (key == "position" && std::holds_alternative<glm::vec3>(val))
                position = std::get<glm::vec3>(val);
            else if (key == "rotation" && std::holds_alternative<glm::vec3>(val))
                rotation = std::get<glm::vec3>(val);
            else if (key == "scale" && std::holds_alternative<glm::vec3>(val))
                scale = std::get<glm::vec3>(val);
        }
    }
};

struct GlobalTransform : public Component {
    glm::mat4 matrix{1.0f};

    GlobalTransform() = default;
    explicit GlobalTransform(const glm::mat4& mat) : matrix(mat) {}
};

struct Parent {
    Entity parent;
};

struct Camera : public Component {
    float fov = 60.0f;
    float aspect = 800.f / 600.f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 getProjection() const {
        return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
    }


    void applyProperties(const Properties& props) {
        for (const auto& [key, val] : props) {
            // if (key == "position" && std::holds_alternative<glm::vec3>(val))
            //     position = std::get<glm::vec3>(val);
            // else if (key == "rotation" && std::holds_alternative<glm::vec3>(val))
            //     rotation = std::get<glm::vec3>(val);
            // else if (key == "scale" && std::holds_alternative<glm::vec3>(val))
            //     scale = std::get<glm::vec3>(val);
        }
    }
};
