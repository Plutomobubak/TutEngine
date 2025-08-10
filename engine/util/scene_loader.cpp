#include "scene_loader.h"
#include <iostream>
#include <filesystem>
#include "../core/component.h"
#include "../core/registry.h"
#include "../core/tutLib.h"

using ComponentHandler = std::function<void(unsigned int, const Properties&)>;
std::unordered_map<std::string, ComponentHandler> componentHandlers;

#define REGISTER_COMPONENT_HANDLER(type) \
componentHandlers[#type] = [](unsigned int e, const Properties& props) { \
    auto& comp = Registry::instance().emplace_or_replace<type>(e); \
    comp.applyProperties(props); \
    comp.init(); \
};

void printVec3(const glm::vec3& v) {
    std::cout << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

void printVariant(const Variant& var) {
    std::visit([](auto&& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, glm::vec3>) {
            printVec3(value);
        } else {
            std::cout << value;
        }
    }, var);
}

void printScene(const Scene& scene) {
    for (const auto& entity : scene.entities) {
        std::cout << "Entity: " << entity.name << "\n";
        for (const auto& comp : entity.components) {
            std::cout << "  Component: " << comp.name << "\n";
            for (const auto& [key, value] : comp.properties) {
                std::cout << "    " << key << ": ";
                printVariant(value);
                std::cout << "\n";
            }
        }
    }
}

void registerComponents() {
    REGISTER_COMPONENT_HANDLER(Transform);
    REGISTER_COMPONENT_HANDLER(Parent);
    REGISTER_COMPONENT_HANDLER(Collider);
    REGISTER_COMPONENT_HANDLER(Camera);
    REGISTER_COMPONENT_HANDLER(Mesh);
componentHandlers["Script"] = [](unsigned int e, const Properties& props) {
    auto& comp = Registry::instance().emplace_or_replace<Script>(e);
    comp.setEntity(e);            // set entity ID here
    comp.applyProperties(props);
    comp.init();
};
    // ... etc
}

void loadPrefabs() {
    std::string path = "project/entities";

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry)) {
            Parser(readFile(entry.path())).parsePrefabEntities();
        }
    }
}

void loadScene() {
    loadPrefabs();
    std::string sceneText = readFile("project/scene.tut");
    if (sceneText.empty()) {
        std::cerr << "Scene file is empty or missing.\n";
        exit(1);
    }
    Parser parser(sceneText);
    Scene scene = parser.parseScene();
    printScene(scene);
    registerComponents();
    for (const auto& entity : scene.entities) {
        unsigned int e = Registry::instance().create(entity.name);
        for (const auto& comp : entity.components) {
            auto it = componentHandlers.find(comp.name);
            if (it != componentHandlers.end()) {
                it->second(e, comp.properties);
            } else {
                std::cerr << "Unknown component: " << comp.name << std::endl;
            }
        }
    }
}
