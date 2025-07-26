#pragma once
#include "animation.h"
#include <vector>
#include <glm/glm.hpp>

class Model; // forward declaration

class AnimationComponent {
public:
    const aiScene* scene = nullptr;
    std::map<std::string, unsigned int> boneMapping;
    std::vector<glm::mat4> boneOffsets;
    std::vector<glm::mat4> finalBoneMatrices;

    Animation* animation = nullptr; // your Animation wrapper class
    bool playing = true;
    bool loop = true;
    float speed = 1.0f;

    double currentTime = 0.0;
    
    std::vector<glm::mat4> boneTransforms;  // final bone transforms for rendering

    AnimationComponent(const Model* model, Animation* anim) {
        scene = model->getScene();
        boneMapping = model->getBoneMapping();
        boneOffsets = model->getBoneOffsets();
        finalBoneMatrices.resize(boneOffsets.size(), glm::mat4(1.0f));
        animation = anim;
        currentTime = 0.0;
    }
};
