/**
 * @file Object.hpp
 * @brief Defines the Object class, a special ECS entity wrapper.
 *
 * Every Object **automatically** includes:
 * - TransformComponent (for position, rotation, and scale).
 * - MeshComponent (to store and manage the object's geometry).
 *
 * This makes Object good for representing planets, vehicles or any other
 * dynamic renderable objects within the simulation.
 *
 * @note If an entity does not require rendering, consider using a base Entity
 * instead.
 */

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "components/MeshComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Entity.hpp"

/**
 * @class Object
 * @brief A high-level ECS entity wrapper with default components.
 *
 * The Object class extends Entity to provide a ready-to-use object
 * with predefined rendering and transform components.
 */
class Object : public Entity {
public:
  Object(Registry &registry, const Mesh &mesh) : Entity(registry) {
    add_component<TransformComponent>();
    add_component<MeshComponent>(mesh);
    transform = get_component<TransformComponent>();
    meshComponent = get_component<MeshComponent>();
  }

  TransformComponent *transform;
  MeshComponent *meshComponent;
};

// TODO  consider creating separate classes such as StaticObject or
// PhysicsObject

#endif // OBJECT_HPP
