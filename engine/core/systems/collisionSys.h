#pragma once

#include "../registry.h"
#include "../components/transform.h"
#include "../components/collider.h"


class CollisionSys{
  public:
void checkCollisions() {
    auto& registry = Registry::instance();

    // Collect all colliders AND reset their flags
    std::vector<std::pair<Entity, Collider*>> colliders;
    registry.view<Collider, GlobalTransform>([&](Entity e, Collider& c, GlobalTransform& gt){
        c.colliding = false; // <-- Reset the flag here
        colliders.push_back({e, &c});
    });

    // Brute-force pairwise check
    for (size_t i = 0; i < colliders.size(); ++i) {
        Collider* a = colliders[i].second;
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            Collider* b = colliders[j].second;

            glm::mat4 tfA = registry.get<GlobalTransform>(colliders[i].first).matrix;
            glm::mat4 tfB = registry.get<GlobalTransform>(colliders[j].first).matrix;

            if (!((a->mask & b->group) && (b->mask & a->group))) {
                continue; // skip if they don't collide by group/mask rules
            }
            if (checkAABBCollision(a->getAABB(tfA), b->getAABB(tfB))) {
                a->colliding = true;
                b->colliding = true;
                std::cout << "Collision detected between "
                          << colliders[i].first << " and " << colliders[j].first << "\n";
            }
        }
    }
}
private:

bool checkAABBCollision(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}
};
