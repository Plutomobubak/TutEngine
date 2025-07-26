#include "tutel.h"


Entity Tutel::tutel;

void Tutel::Init(){
    // Load model
    // Model* model = Model::loadModel("assets/meshy_char_rigged.fbx");
    // if (!model) {
    //     std::cerr << "Model loading failed!\n";
    //     return;
    // }
    
    // Assuming Animation has a constructor like Animation(const aiAnimation*)
    // Animation* animation = nullptr;
    // if (model->getScene() && model->getScene()->mNumAnimations > 0) {
    //     animation = new Animation(model->getScene()->mAnimations[0]); // take first animation
    // }

    auto& reg = Registry::instance();
    tutel = reg.create("TutelR");
    reg.get<Transform>(tutel).position = glm::vec3(0,0,1);
    // reg.get<Transform>(e).rotation = glm::vec3(0,0,-1.55);
    // reg.add<Mesh>(tutel, Mesh{
    //     model
    //     });
    // if (animation) {
    //     reg.add<animationcomponent>(tutel, animationcomponent(model, animation));
    // }
}


void Tutel::Update(float deltaT){
    auto& reg = Registry::instance();
      reg.get<Transform>(tutel).rotation += glm::vec3(0,1,0) * deltaT;
}
