#include "../../engine/core/tutLib.h"  // your common interface header

class TestScript : public TutScript {
public:
    void Init() override {
        std::cout << "[Script] Init called\n";
        std::cout << "Test script initialized for entity " << GetEntity() << "\n";
        // Access transform, registry, etc. here if you expose them
    }

    void Update(float dt) override {
        // std::cout << "[Script] Update called with dt = " << dt << "\n";
    }
};

// Entry point expected by the engine
extern "C" TutScript* CreateScript() {
    return new TestScript();
}
