#pragma once

#include <variant>
#include <string>
#include <glm/glm.hpp>

using Variant = std::variant<bool, int, float, std::string, glm::vec3>;
using Properties = std::unordered_map<std::string, Variant>;

struct Component {
    unsigned int entity;
    virtual ~Component() = default;
    virtual void init() {};
    virtual void update(float dt) {};
    virtual void applyProperties(const Properties& props) {};
};
