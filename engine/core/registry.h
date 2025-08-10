#pragma once
#include <glm/glm.hpp>
#include "components/transform.h"
#include <unordered_map>
#include <typeindex>
#include <tuple>
#include <iostream>
#include <cxxabi.h>  // for __cxa_demangle (GCC/Clang only)

using Entity = unsigned int;

class Registry {
private:
    // Your data members as usual
    Entity nextEntity = 1;
    std::unordered_map<std::type_index, std::unordered_map<Entity, void*>> components;
    std::vector<std::type_index> registeredTypes;
    std::unordered_map<std::string, Entity> entityNameMap;  // For lookup by name

    // Private constructor so no one else can create an instance
    Registry() = default;

public:
    // Delete copy constructor/assignment to avoid multiple instances
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;

    // Static accessor for the single instance
    static Registry& instance() {
        static Registry instance;
        return instance;
    }

    void debugPrintComponents() const {
        std::cout << "\n--- Component Registry Debug ---\n";

        for (const auto& [type, entityMap] : components) {
            // Demangle type name
            int status;
            const char* rawName = type.name();
            char* demangled = abi::__cxa_demangle(rawName, 0, 0, &status);
            std::string typeName = (status == 0 && demangled) ? demangled : rawName;
            free(demangled);

            std::cout << "Component Type: " << typeName << "\n";
            for (const auto& [entity, ptr] : entityMap) {
                std::cout << "  Entity ID: " << entity << "\n";
            }
        }

        std::cout << "--------------------------------\n";
    }
    // Methods now non-static, but called via instance()

    Entity create(const std::string& name = "") {
        Entity e = nextEntity++;
        add<Transform>(e, Transform(glm::vec3(0,0,0),glm::vec3(0,0,0),glm::vec3(1,1,1)));
        add<GlobalTransform>(e, GlobalTransform{});
        if (!name.empty()) {
            entityNameMap[name] = e;
        }
        return e;
    }

    template<typename T>
    void add(Entity e, T component) {
        auto& storage = components[typeid(T)];
        if (storage.empty()) {
            registeredTypes.push_back(typeid(T));
        }
        component.entity = e;
        storage[e] = new T(std::move(component));
    }

    template<typename T>
    T& get(Entity e) {
        return *reinterpret_cast<T*>(components[typeid(T)][e]);
    }

    template<typename T>
    T* try_get(unsigned int entity) {
        auto& storage = components[typeid(T)];
        auto it = storage.find(entity);
        if (it != storage.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }
    template<typename T>
    bool has(Entity e) const {
        auto it = components.find(typeid(T));
        return it != components.end() && it->second.find(e) != it->second.end();
    }

    // Find entity by name
    Entity find(const std::string& name) {
        auto it = entityNameMap.find(name);
        if (it != entityNameMap.end()) {
            return it->second;
        }
        return 0; // or some invalid entity value
    }
    template<typename... Components, typename Func>
    void view(Func func) {
        if constexpr (sizeof...(Components) == 0) return;

        using First = std::tuple_element_t<0, std::tuple<Components...>>;
        auto it = components.find(typeid(First));
        if (it == components.end()) return;

        auto& firstStorage = it->second;

        for (auto& [entity, compPtr] : firstStorage) {
            // Check if entity has all other components
            if ((has<Components>(entity) && ...)) {
                func(entity, get<Components>(entity)...);
            }
        }
    }
template<typename T, typename... Args>
T& emplace_or_replace(Entity e, Args&&... args) {
    auto& storage = components[typeid(T)];
    if (storage.empty()) {
        registeredTypes.push_back(typeid(T));
    }
    if (storage.find(e) != storage.end()) {
        // Replace existing component
        delete static_cast<T*>(storage[e]);
        storage[e] = new T(std::forward<Args>(args)...);
    } else {
        storage[e] = new T(std::forward<Args>(args)...);
    }
    
    T* comp = static_cast<T*>(storage[e]);
    comp->entity = e;  // Set entity member here
    
    return *comp;
}

    void updateAllComponents(float dt) {
        for (auto& type : registeredTypes) {
            auto& storage = components[type];

            for (auto& [entity, ptr] : storage) {
                Component* comp = static_cast<Component*>(ptr);
                comp->update(dt);
            }
        }
    }
};
