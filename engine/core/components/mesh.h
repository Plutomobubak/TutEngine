#pragma once
#include <iostream>
#include "../../renderer/model.h"
#include "../component.h"

struct Mesh : public Component {
    Model* model;
    std::string src;

    void applyProperties(const Properties& props) override {
        for (const auto& [key, val] : props) {
            if (key == "src" && std::holds_alternative<std::string>(val))
                src = std::get<std::string>(val);
        }
    }

    void init() override {
      model = Model::loadModel(src);
      std::cout << src << std::endl;
      if (!model) {
          std::cerr << "Model loading failed!\n";
          return;
      }
    }
};
