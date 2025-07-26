#pragma once
#include "../components/animationComponent.h"
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>

class AnimationSys {
public:
    // Updates all animation components (pass deltaTime in seconds)
    void update(std::vector<AnimationComponent*>& components, double deltaTime, const aiNode* rootNode);

private:
    void calculateNodeTransforms(
        const aiNode* node,
        const glm::mat4& parentTransform,
        double animationTime,
        const Animation& animation,
        std::map<std::string, glm::mat4>& finalTransforms);
};
