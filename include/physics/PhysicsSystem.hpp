#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Registry.hpp"
#include <iostream>

class PhysicsSystem {
public:
  static void update(Registry &registry, float dt) {
    for (Entity entity :
         registry.get_entities_with_component<PhysicsComponent>()) {
      if (!registry.has_component<TransformComponent>(entity))
        continue;

      auto &transform = registry.get_component<TransformComponent>(entity);
      auto &velocity = registry.get_component<PhysicsComponent>(entity);

      transform.position = transform.position + velocity.velocity * dt;
    }
  }
};

#endif // PHYSICS_SYSTEM_HPP
