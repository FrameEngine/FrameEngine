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

#include "Entity.hpp"
#include "components/TransformComponent.hpp"
#include "rendering/Mesh.hpp"

// Forward declare Renderer to avoid circular dependency
class Renderer;

/**
 * @class Object
 * @brief A high-level ECS entity wrapper with default components.
 *
 * The Object class extends Entity to provide a ready-to-use object
 * with predefined components.
 */
class Object : public Entity {
private:
  Mesh *mesh;
  Vector3 color;

public:
  Object(Registry &registry, Mesh *mesh)
      : Entity(registry), mesh(mesh), color(Vector3(1.0f, .5f, 1.0f)) {
    add_component<TransformComponent>();
    transform = get_component<TransformComponent>();
  }

  TransformComponent *transform;

  /**
   * @brief Rotates the object.
   * @param axis The axis to rotate around.
   * @param angle The angle in degrees.
   */
  void rotate(const Vector3 &axis = Vector3(0, 1, 0), float angle = 1.0f) {
    transform->rotation =
        Quaternion::from_axis_angle(axis, angle) * transform->rotation;
  }

  /**
   * @brief Moves the object in world space.
   * @param translation The translation vector.
   */
  void move(const Vector3 &translation) {
    transform->position = transform->position + translation;
  }

  /**
   * @brief Updates the model matrix and draws the mesh.
   */
  void render(Renderer &renderer);

  Vector3 getColor() const { return color; }
  void setColor(const Vector3 &c) { color = c; }

  /**
   * @brief Makes the camera look at a target.
   * @param target Target position
   */
  void lookAt(const Vector3 &target) {
    Vector3 direction = (target - transform->position).normalized();

    if (direction.magnitude() < 0.0001f) {
      LOG(WARNING, "lookAt() called with the same position, ignoring.");
      return;
    }

    Vector3 up(0.f, 1.f, 0.f);

    // Handle gimbal lock
    if (fabs(direction.dot(up)) > 0.999f) {
      up = Vector3(0.f, 0.f, 1.f);
    }

    Vector3 right = up.cross(direction).normalized();
    Vector3 adjustedUp = direction.cross(right);

    Matrix4 rotationMatrix = Matrix4({
        right.x, adjustedUp.x, direction.x, 0.0f, //
        right.y, adjustedUp.y, direction.y, 0.0f, //
        right.z, adjustedUp.z, direction.z, 0.0f, //
        0.0f, 0.0f, 0.0f, 1.0f                    //
    });

    transform->rotation = Quaternion::fromMatrix(rotationMatrix);
  }
};

// TODO  consider creating separate classes such as StaticObject or
// PhysicsObject

#endif // OBJECT_HPP
