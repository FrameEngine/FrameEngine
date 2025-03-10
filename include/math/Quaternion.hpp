/**
 * @file Quaternion.hpp
 * @brief Defines the Quaternion class for representing 3D rotations.
 *
 */

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3.hpp"
#include <cmath>

namespace FrameEngine {

// Forward declaration
class Matrix4;

struct Quaternion {
  float w, x, y, z;

  /**
   * @brief Default constructor initializes the quaternion as the identity
   * rotation.
   */
  Quaternion() : w(1), x(0), y(0), z(0) {}

  /**
   * @brief Constructs a quaternion with the given components.
   * @param w The scalar component.
   * @param x The x component of the vector part.
   * @param y The y component of the vector part.
   * @param z The z component of the vector part.
   */
  Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

  /**
   * @return The negated quaternion.
   */
  Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }

  /**
   * @brief Adds two quaternions.
   *
   * @param q The quaternion to add.
   * @return The sum of this quaternion and q.
   */
  Quaternion operator+(const Quaternion &q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
  }

  /**
   * @brief Subtracts one quaternion from another.
   *
   * @param q The quaternion to subtract.
   * @return The difference between this quaternion and q.
   */
  Quaternion operator-(const Quaternion &q) const {
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
  }

  /**
   * @brief Multiplies the quaternion by a scalar.
   *
   * @param scalar The scalar value to multiply.
   * @return A new quaternion scaled by the given scalar.
   */
  Quaternion operator*(float scalar) const {
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
  }

  /**
   * @brief Returns a normalized version of this quaternion.
   *
   * @return A new quaternion that is the normalized (unit length) version of
   * this quaternion.
   */
  Quaternion normalized() const {
    float mag = std::sqrt(w * w + x * x + y * y + z * z);
    return {w / mag, x / mag, y / mag, z / mag};
  }

  /**
   * @brief Creates a quaternion from an axis-angle representation.
   *
   * Constructs a quaternion representing a rotation of the given angle (in
   * degrees) about the specified axis.
   *
   * @param axis The axis of rotation.
   * @param angle The rotation angle in degrees.
   * @return A quaternion representing the rotation.
   */
  static Quaternion fromAxisAngle(const Vector3 &axis, float angle) {
    float rad = angle * M_PI / 180.0f;
    float sinHalf = -std::sin(rad / 2);
    Vector3 normalizedAxis = axis.normalized();
    return Quaternion(std::cos(rad / 2),          //
                      normalizedAxis.x * sinHalf, //
                      normalizedAxis.y * sinHalf, //
                      normalizedAxis.z * sinHalf  //
    );
  }

  /**
   * @brief Computes the dot product of two quaternions.
   *
   * @param q The other quaternion.
   * @return The dot product.
   */
  float dot(const Quaternion &q) const {
    return x * q.x + y * q.y + z * q.z + w * q.w;
  }

  /**
   * @brief Multiplies two quaternions.
   *
   * Performs Hamilton product of this quaternion with another.
   *
   * @param q The quaternion to multiply with.
   * @return The resulting quaternion.
   */
  Quaternion operator*(const Quaternion &q) const {
    return {w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w};
  }

  /**
   * @brief Rotates a vector by this quaternion.
   *
   * The rotation is applied by converting the vector into a quaternion,
   * performing the quaternion multiplication, and then extracting the rotated
   * vector.
   *
   * @param v The vector to rotate.
   * @return The rotated vector.
   */
  Vector3 rotateVector(const Vector3 &v) const {
    Quaternion normalized = this->normalized();
    Quaternion vecQuat(0, v.x, v.y, v.z);
    Quaternion invQuat(normalized.w, -normalized.x, -normalized.y,
                       -normalized.z); // Conjugate

    Quaternion result = normalized * vecQuat * invQuat;

    return Vector3(result.x, result.y, result.z);
  }

  /**
   * @brief Converts this quaternion to a rotation matrix.
   *
   * @return A Matrix4 representing the equivalent rotation.
   */
  Matrix4 toMatrix() const;
  static Quaternion fromMatrix(const Matrix4 &matrix);

  /**
   * @brief Creates a "look rotation" quaternion.
   *
   * Generates a quaternion that rotates an object to face in the specified
   * direction.
   *
   * @param direction The direction to look at.
   * @param up The up vector. Defaults to (0, 1, 0).
   * @return The look rotation quaternion.
   */
  static Quaternion lookRotation(const Vector3 &direction,
                                 const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));
};
} // namespace FrameEngine

#endif // QUATERNION_HPP
