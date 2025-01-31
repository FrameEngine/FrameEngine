#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include "math/Vector3.hpp"

struct PhysicsComponent {
  Vector3 velocity;
  Vector3 acceleration;

  PhysicsComponent(Vector3 vel = Vector3(), Vector3 acc = Vector3())
      : velocity(vel), acceleration(acc) {}
};

#endif // PHYSICS_COMPONENT_HPP
