#pragma once

#include "../registry.h"
#include "../components/transform.h"
#include "../components/collider.h"



AABB Collider::getAABB(const glm::mat4& transform) const {
    glm::vec3 center = glm::vec3(transform[3]); // translation
    glm::vec3 halfSize = size * 0.5f;
    return { center - halfSize, center + halfSize };
}

class CollisionSys{
  public:
void checkCollisions() {
    auto& registry = Registry::instance();

    // Collect all colliders
    std::vector<std::pair<Entity, Collider*>> colliders;
    registry.view<Collider, GlobalTransform>([&](Entity e, Collider& c, GlobalTransform& gt){
        colliders.push_back({e, &c});
    });

    // Brute-force pairwise check
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            Collider* a = colliders[i].second;
            Collider* b = colliders[j].second;

            glm::mat4 tfA = registry.get<GlobalTransform>(colliders[i].first).matrix;
            glm::mat4 tfB = registry.get<GlobalTransform>(colliders[j].first).matrix;

            if (checkAABBCollision(a->getAABB(tfA), b->getAABB(tfB))) {
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
