/**
 * @file PhysicsComponent.hpp
 * @brief Defines the PhysicsComponent for ECS-based physics simulation.
 */

#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include "math/Vector3.hpp"

namespace FrameEngine {

/**
 * @struct PhysicsComponent
 * @brief Represents the physical properties of an entity in the ECS.
 *
 * This component is used in physics simulations to track an entity’s
 * velocity, acceleration, and mass, allowing it to respond to forces
 * and gravity.
 */
struct PhysicsComponent {
  Vector3 velocity;
  Vector3 acceleration;
  float mass;

  /**
   * @brief Constructs a PhysicsComponent with optional initial values.
   * @param vel Initial velocity of the entity (default: zero vector).
   * @param acc Initial acceleration of the entity (default: zero vector).
   * @param m Mass of the entity (default: 1.0).
   *
   */
  PhysicsComponent(Vector3 vel = Vector3(), Vector3 acc = Vector3(),
                   float m = 1.0f)
      : velocity(vel), acceleration(acc), mass(m) {}
};

} // namespace FrameEngine

#endif // PHYSICS_COMPONENT_HPP
