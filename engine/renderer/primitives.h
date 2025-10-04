#pragma once
#include "model.h"

// Simple procedural primitives for testing / prototyping
namespace Primitives {
    Model* CreateCube(float size = 1.0f);
    Model* CreatePlane(float size = 1.0f);
    Model* CreateSphere(float radius = 1.0f, unsigned int sectors = 16, unsigned int stacks = 16);
    // Model* CreateCapsule(float radius = 0.5f, float height = 2.0f, unsigned int segments = 16);
}
