/**
 * @file Object.hpp
 * @brief Defines the Object class, which serves as a wrapper for ECS entities.
 *
 * This class provides an abstraction over the ECS system, allowing objects to
 * be created and manipulated without directly interfacing with the `Registry`.
 * Transform and Mesh components are attached by default since every object
 * in the simulation should have them.
 *
 * Object can represent a planet, vehicle, or any dynamic entity
 * in the simulation. But also can be extended with different components
 *
 */

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "components/MeshComponent.hpp"
#include "components/TransformComponent.hpp"
#include "core/Registry.hpp"
#include <iostream>

// TODO Refactor this into a "BaseObject" class that does not include default
// components. And extend it with "Body" class or smth like this

/**
 * @class Object
 * @brief A high-level ECS entity wrapper with default components.
 *
 * Every 'Object' automatically has:
 * - 'TransformComponent' (handles position, rotation, and scale).
 * - 'MeshComponent' (stores mesh data for rendering).
 *
 */
class Object {
public:
  /**
   * @brief Constructs an Object with a default transform and mesh.
   * @param registry Reference to the ECS registry managing entities.
   * @param mesh The mesh to associate with this object.
   *
   * This constructor:
   * - Creates a new entity in the ECS.
   * - Automatically attaches 'TransformComponent' and 'MeshComponent'.
   * - Caches pointers to these components for fast access. (transform,
   * meshComponent)
   */
  Object(Registry &registry, const Mesh &mesh) : registry_(registry) {

    id_ = registry_.create_entity();

    add_component<TransformComponent>(TransformComponent());
    add_component<MeshComponent>(MeshComponent(mesh));

    transform = get_component<TransformComponent>();
    meshComponent = get_component<MeshComponent>();
  }
  /**
   * @brief Adds a component of type `T` to the object.
   * @tparam T The component type.
   * @tparam Args Variadic template arguments for the component constructor.
   * @param args Arguments forwarded to the component constructor.
   *
   * If the entity already has this component, the function logs a warning.
   */
  template <typename T, typename... Args> void add_component(Args &&...args) {
    if (registry_.has_component<T>(id_)) {
      std::cerr << "Warning: Entity " << id_ << " already has component "
                << typeid(T).name() << std::endl;
      return;
    }
    registry_.add_component<T>(id_, T(std::forward<Args>(args)...));
  }

  /**
   * @brief Removes a component of type 'T' from the object.
   * @tparam T The component type to remove.
   *
   * If the component does not exist, a warning is logged.
   */
  template <typename T> void remove_component() {
    if (!registry_.has_component<T>(id_)) {
      std::cerr << "Warning: Entity " << id_ << " does not have component "
                << typeid(T).name() << std::endl;
      return;
    }
    registry_.remove_component<T>(id_);
  }

  /**
   * @brief Retrieves a pointer to the specified component type 'T'.
   * @tparam T The component type to retrieve.
   * @return A pointer to the component, or `nullptr` if not found.
   *
   * This function provides kinda safe component access by returning nullptr
   * if the entity does not have the requested component.
   */
  template <typename T> T *get_component() {
    if (!registry_.has_component<T>(id_)) {
      return nullptr; // component doesn't exist
    }
    return &registry_.get_component<T>(id_);
  }

  // Cached, as every object has them
  TransformComponent *transform;
  MeshComponent *meshComponent;

  /**
   * @brief Returns the unique entity ID of this object.
   * @return The entity ID.
   */
  Entity id() const { return id_; }

private:
  Entity id_;
  Registry &registry_;
};

#endif // OBJECT_HPP
