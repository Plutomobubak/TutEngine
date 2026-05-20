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
        else if (key == "group" && std::holds_alternative<int>(val)) {
            group = static_cast<uint32_t>(std::get<int>(val));
        }
        else if (key == "mask" && std::holds_alternative<int>(val)) {
            mask = static_cast<uint32_t>(std::get<int>(val));
        }
    }
}
AABB Collider::getAABB(const glm::mat4& transform) const {
    glm::vec3 half = size * 0.5f;

    // Extract basis vectors (columns 0–2)
    glm::vec3 right = glm::vec3(transform[0]) * half.x;
    glm::vec3 up    = glm::vec3(transform[1]) * half.y;
    glm::vec3 forward = glm::vec3(transform[2]) * half.z;

    // Translation
    glm::vec3 center = glm::vec3(transform[3]);

    // Compute world-space AABB extents using the absolute value of rotation components
    glm::vec3 worldHalf = glm::vec3(
        fabs(right.x) + fabs(up.x) + fabs(forward.x),
        fabs(right.y) + fabs(up.y) + fabs(forward.y),
        fabs(right.z) + fabs(up.z) + fabs(forward.z)
    );

    return { center - worldHalf, center + worldHalf };
}
