#include "collider.h"
#include "../../renderer/wireframe.h"
#include "../registry.h"

Collider::~Collider() = default;
void Collider::init() {
  setupWireframeMesh();
}
void Collider::update(float dt){
    auto& transform = Registry::instance().get<GlobalTransform>(entity);
    if (debug){
      drawWireframe(transform.matrix,size);
    }
}
void Collider::applyProperties(const Properties& props) {
    for (const auto& [key, val] : props) {
        if (key == "type" && std::holds_alternative<std::string>(val)) {
            std::string value = std::get<std::string>(val);
            if (value == "box") {
                type = ColliderType::Box;
            } else if (value == "capsule") {
                type = ColliderType::Capsule;
            } else if (value == "sphere") {
                type = ColliderType::Sphere;
            }
        }
        else if (key == "size" && std::holds_alternative<glm::vec3>(val)) {
            size = std::get<glm::vec3>(val);
        }
        else if (key == "debug" && std::holds_alternative<bool>(val)) {
            debug = std::get<bool>(val);
        }
    }
}
