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
  // TODO move it to Renderer component
  Mesh *mesh;             ///< Pointer to the mesh.
  bool wireframe = false; ///< Flag indicating wether the object should be
                          ///< rendered in wireframe mode.
  Vector3 color;          ///< The color used when rendering the object.

public:
  /**
   * @brief Constructs an Object with the specified mesh.
   *
   * This constructor automatically adds a TransformComponent to the object and
   * retrieves a pointer to it for convenience.
   *
   * @param registry The ECS registry where the object is registered.
   * @param mesh Pointer to the Mesh that defines the object's geometry.
   */
  Object(Registry &registry, Mesh *mesh)
      : Entity(registry), mesh(mesh), color(Vector3(1.0f, .5f, 1.0f)) {
    add_component<TransformComponent>();
    transform = get_component<TransformComponent>();
  }

  TransformComponent *transform;

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
   * @brief Renders the object.
   *
   * This method updates the model matrix and draws the object's mesh.
   *
   * @param renderer The Renderer used to draw the object.
   */
  void render(Renderer &renderer);

  /**
   * @brief Retrieves the object's render color.
   *
   * @return The color vector used for rendering.
   */
  Vector3 getColor() const { return color; }

  /**
   * @brief Sets the object's render color.
   *
   * @param c The color vector to set.
   */
  // TODO instead of Vector3 use custom Color datatype, that works with 0-255
  void setColor(const Vector3 &c) { color = c; }

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

  /**
   * @brief Enables or disables wireframe rendering for this object.
   *
   * @param enable True to enable wireframe mode; false to disable.
   */
  void setWireframe(bool enable) { wireframe = enable; }

  /**
   * @brief Checks if wireframe rendering is enabled for this object.
   *
   * @return True if wireframe mode is enabled; false otherwise.
   */
  bool isWireframe() const { return wireframe; }
};

#endif // OBJECT_HPP
