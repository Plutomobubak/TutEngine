#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <unordered_map>
#include <string>
#include <functional>
#include <glm/vec3.hpp>
#include <variant>
#include "parser.h"  // make sure this includes needed declarations
#include "utils.h"

using ComponentHandler = std::function<void(unsigned int, const Properties&)>;

extern std::unordered_map<std::string, ComponentHandler> componentHandlers;

void printVec3(const glm::vec3& v);
void printVariant(const Variant& var);
void printScene(const Scene& scene);
void registerComponents();
void loadPrefabs();
void loadScene();

#endif // SCENE_LOADER_H
