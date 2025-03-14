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
#include "components/MaterialComponent.hpp"
#include "components/MeshComponent.hpp"
#include "components/TransformComponent.hpp"
#include "rendering/Material.hpp"
#include "rendering/Mesh.hpp"

namespace FrameEngine {

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
public:
  /**
   * @brief Constructs an Object with the specified mesh.
   *
   * This constructor automatically adds a TransformComponent to the object and
   * retrieves a pointer to it for convenience.
   *
   * @param mesh Pointer to the Mesh that defines the object's geometry.
   */
  Object(Mesh *mesh) : Entity() {
    add_component<TransformComponent>();
    add_component<MeshComponent>(mesh);
    add_component<MaterialComponent>();

    transform = get_component<TransformComponent>();
    meshComponent = get_component<MeshComponent>();
    materialComponent = get_component<MaterialComponent>();
  }

  TransformComponent *transform;
  MeshComponent *meshComponent;
  MaterialComponent *materialComponent;

  void setMesh(Mesh *mesh, bool wireframe = false);
  void setMaterial(Material *material);

  /**
   * @brief Rotates the object around the specified axis.
   *
   * The object's current rotation is updated by multiplying it with a rotation
   * derived from the given axis-angle representation.
   *
   * @param axis The axis to rotate around (default is (0, 1, 0)).
   * @param angle The rotation angle in degrees (default is 1.0).
   */
  void rotate(const Vector3 &axis = Vector3(0, 1, 0), float angle = 1.0f) {
    transform->rotation =
        Quaternion::fromAxisAngle(axis, angle) * transform->rotation;
  }

  /**
   * @brief Translates the object in world space.
   *
   * The object's position is updated by adding the translation vector.
   *
   * @param translation The vector by which to translate the object.
   */
  void move(const Vector3 &translation) {
    transform->position = transform->position + translation;
  }

  /**
   * @brief Orients the object so that it "looks at" a target position.
   *
   * This method updates the object's rotation so that its forward direction
   * points toward the target. It uses a look-at algorithm to compute a rotation
   * matrix from the current position to the target, which is then converted
   * into a quaternion and applied to the transform.
   *
   * @param target The target position to look at.
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

} // namespace FrameEngine

#endif // OBJECT_HPP
