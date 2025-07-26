#pragma once
#include "../registry.h"
#include "../components/transform.h"

class GlobalTransformSystem {
public:
    static void update() {
        auto& reg = Registry::instance();

        std::function<void(Entity, const glm::mat4&)> recurse =
            [&](Entity e, const glm::mat4& parentMatrix) {
                if (!reg.has<Transform>(e)) return;

                const auto& local = reg.get<Transform>(e);
                glm::mat4 localMat = local.getLocalMatrix();
                glm::mat4 globalMat = parentMatrix * localMat;

                if (reg.has<GlobalTransform>(e))
                    reg.get<GlobalTransform>(e).matrix = globalMat;
                else
                    reg.add<GlobalTransform>(e, GlobalTransform{globalMat});

                reg.view<Parent, Transform>([&](Entity child, Parent& p, Transform&) {
                    if (p.parent == e)
                        recurse(child, globalMat);
                });
            };

        // Start with all root entities (no Parent component)
        reg.view<Transform>([&](Entity e, Transform&) {
            if (!reg.has<Parent>(e)) {
                recurse(e, glm::mat4(1.0f));
            }
        });
    }
};
