#include "transform.h"
#include "../registry.h"

Transform::~Transform() = default;
Parent::~Parent() = default;
Camera::~Camera() = default;
void Transform::init() {}
void Transform::update(float dt) {}
void Transform::applyProperties(const Properties& props) {
    for (const auto& [key, val] : props) {
        if (key == "position" && std::holds_alternative<glm::vec3>(val))
            position = std::get<glm::vec3>(val);
        else if (key == "rotation" && std::holds_alternative<glm::vec3>(val))
            rotation = std::get<glm::vec3>(val);
        else if (key == "scale" && std::holds_alternative<glm::vec3>(val))
            scale = std::get<glm::vec3>(val);
    }
}

void Parent::init() {}
void Parent::update(float dt) {}
void Parent::applyProperties(const Properties& props) {
    for (const auto& [key, val] : props) {
        if (key == "parent" && std::holds_alternative<std::string>(val))
            parent = Registry::instance().find(std::get<std::string>(val));
    }
}

void Camera::init() {}
void Camera::update(float dt) {}
void Camera::applyProperties(const Properties& props) {
    for (const auto& [key, val] : props) {
        if (key == "fov" && std::holds_alternative<float>(val))
            fov = std::get<float>(val);
        else if (key == "aspect" && std::holds_alternative<float>(val))
            aspect = std::get<float>(val);
        else if (key == "nearPlane" && std::holds_alternative<float>(val))
            nearPlane = std::get<float>(val);
        else if (key == "farPlane" && std::holds_alternative<float>(val))
            farPlane = std::get<float>(val);
    }
}
