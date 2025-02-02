/**
 * @file TransformComponent.hpp
 * @brief Defines the TransformComponent for ECS-based spatial transformations.
 *
 *This component is used in rendering, physics, and game logic to determine an
 *entity's placement in the world.
 */

#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

/**
 * @struct TransformComponent
 * @brief Represents the transformation properties of an entity.
 *
 * The `TransformComponent` stores an entity’s position, rotation, and scale.
 */
struct TransformComponent {
  /// The world position
  Vector3 position;
  Quaternion rotation;
  Vector3 scale;

  /**
   * @brief Default constructor that initializes an identity transform.
   *
   * - Position is zero-vector. Entity starts at the
   * origin.
   * - Rotation is initialized to an identity quaternion (1,0,0,0). No
   * rotation.
   * - Scale is (1, 1, 1).
   */
  TransformComponent()
      : position(0.0f, 0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f),
        scale(1.0f, 1.0f, 1.0f) {}

  /**
   * @brief Constructs a TransformComponent with custom position, rotation, and
   * scale.
   * @param pos The initial position of the entity.
   * @param rot The initial rotation (default: identity quaternion).
   * @param scl The initial scale (default: (1,1,1)).
   *
   * Allows initialization with specific transformation values.
   */
  TransformComponent(const Vector3 &pos, const Quaternion &rot = Quaternion(),
                     const Vector3 &scl = Vector3(1.0f, 1.0f, 1.0f))
      : position(pos), rotation(rot), scale(scl) {}

  /**
   * @brief Computes the transformation matrix for the entity.
   * @return A `Matrix4` representing the entity’s world transformation.
   */
  Matrix4 get_transformation_matrix() const {
    return rotation.toMatrix() * Matrix4::from_translation(position) *
           Matrix4::from_scaling(scale);
  }
};

#endif // TRANSFORM_COMPONENT_HPP
