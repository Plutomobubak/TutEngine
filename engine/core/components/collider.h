#pragma once
#include <iostream>
#include <string>
#include "../component.h"
#include <glm/glm.hpp>

struct Collider : public Component {
    enum ColliderType {
      Box,
      Capsule,
      Sphere
    };
    ColliderType type = Box;
    glm::vec3 size = glm::vec3(1.0f);
    bool debug = false;

    void applyProperties(const Properties& props) override ;
    
    void init() override ;
    void update(float dt) override ;
    ~Collider() override ;
};
