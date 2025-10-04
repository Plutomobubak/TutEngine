#pragma once
#include <iostream>
#include <variant>
#include <string>
#include "../../renderer/model.h"
#include "../../renderer/primitives.h"
#include "../component.h"

// Your property map type should look like: using properties = std::unordered_map<std::string, std::variant<int,float,std::string>>;
struct Primitive : public Component {
    Model* model = nullptr;
    std::string type = "Cube";  // default
    std::string texture = "";
    float size = 1.0f;
    float radius = 0.5f;
    float height = 2.0f;
    int segments = 16;

    void applyProperties(const Properties& props) override {
        for (const auto& [key, val] : props) {
            if (key == "type" && std::holds_alternative<std::string>(val))
                type = std::get<std::string>(val);
            else if (key == "size" && std::holds_alternative<float>(val))
                size = std::get<float>(val);
            else if (key == "radius" && std::holds_alternative<float>(val))
                radius = std::get<float>(val);
            else if (key == "height" && std::holds_alternative<float>(val))
                height = std::get<float>(val);
            else if (key == "segments" && std::holds_alternative<int>(val))
                segments = std::get<int>(val);
            else if (key == "texture" && std::holds_alternative<std::string>(val))
                texture = std::get<std::string>(val);
        }
    }

    void init() override {
        using namespace Primitives;

        if (type == "Cube") {
            model = CreateCube(size);
        } else if (type == "Plane") {
            model = CreatePlane(size);
        } else if (type == "Sphere") {
            model = CreateSphere(radius, segments, segments);
        // } else if (type == "Capsule") {
        //     model = CreateCapsule(radius, height, segments);
        } else {
            std::cerr << "Unknown primitive type: " << type << std::endl;
            return;
        }

        if (!model) {
            std::cerr << "Failed to create primitive model: " << type << std::endl;
            return;
        }
        if (!texture.empty()) {
          model->setTexture(texture);
        }

        std::cout << "Created primitive: " << type << std::endl;
    }

    void draw() {
        if (model) model->draw();
    }
};
