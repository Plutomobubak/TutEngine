#pragma once

#include "../core/registry.h"
#include "../core/components/transform.h"
#include "../core/systems/inputManager.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

class Player {
  public:
    static void Init();
    static void Update(float deltaT);

    static Entity getEntity(){return player;};

  private:
    static Entity player;

    static double lastX;
  static double lastY;
  static bool firstMouse;

    static void Move(Registry& reg, float deltaT);
    static void LookAround(Registry& reg, float deltaT);
};
