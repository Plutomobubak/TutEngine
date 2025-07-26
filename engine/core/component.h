#pragma once

#include <variant>
#include <string>
#include <glm/glm.hpp>

using Variant = std::variant<int, float, std::string, glm::vec3>;
using Properties = std::unordered_map<std::string, Variant>;

struct Component {
    virtual ~Component() = default;
    virtual void init() {};
    virtual void update(float dt) {};
    virtual void applyProperties(const Properties& props) {};
};
