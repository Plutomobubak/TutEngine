#include <iostream>

#include "../core/registry.h"
#include "../core/components/mesh.h"
#include "../renderer/model.h"


class Tutel{
  public:
    static void Init();
    static void Update(float deltaT);

    static Entity getEntity(){return tutel;};
  private:
    static Entity tutel;
};
