#pragma once
#include <map>
#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Animation {
public:
    double duration;
    double ticksPerSecond;
    std::map<std::string, aiNodeAnim*> nodeAnimMap;

    Animation(const aiAnimation* animation);

    // Helper interpolation functions (could also be free functions)
    glm::vec3 interpolatePosition(double time, aiNodeAnim* channel) const;
    glm::quat interpolateRotation(double time, aiNodeAnim* channel) const;
    glm::vec3 interpolateScaling(double time, aiNodeAnim* channel) const;
};
