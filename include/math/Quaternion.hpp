#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3.hpp"
#include <cmath>

struct Matrix4;

struct Quaternion {
  float w, x, y, z;

  Quaternion() : w(1), x(0), y(0), z(0) {}

  Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

  Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }

  Quaternion operator+(const Quaternion &q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
  }

  Quaternion operator-(const Quaternion &q) const {
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
  }

  Quaternion operator*(float scalar) const {
    return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
  }

  Quaternion normalized() const {
    float mag = std::sqrt(w * w + x * x + y * y + z * z);
    return {w / mag, x / mag, y / mag, z / mag};
  }

  static Quaternion from_axis_angle(const Vector3 &axis, float angle) {
    float rad = angle * M_PI / 180.0f;
    float sinHalf = -std::sin(rad / 2);
    Vector3 normalizedAxis = axis.normalized();
    return Quaternion(std::cos(rad / 2),          //
                      normalizedAxis.x * sinHalf, //
                      normalizedAxis.y * sinHalf, //
                      normalizedAxis.z * sinHalf  //
    );
  }

  float dot(const Quaternion &q) const {
    return x * q.x + y * q.y + z * q.z + w * q.w;
  }

  Quaternion operator*(const Quaternion &q) const {
    return {w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w};
  }

  Vector3 rotateVector(const Vector3 &v) const {
    Quaternion normalized = this->normalized();
    Quaternion vecQuat(0, v.x, v.y, v.z);
    Quaternion invQuat(normalized.w, -normalized.x, -normalized.y,
                       -normalized.z); // Conjugate

    Quaternion result = normalized * vecQuat * invQuat;

    return Vector3(result.x, result.y, result.z);
  }

  // Create a rotation matrix from a quaternion
  Matrix4 toMatrix() const;
  static Quaternion fromMatrix(const Matrix4 &matrix);

  /**
   * @brief Creates a quaternion that rotates vector `from` to vector `to`.
   * @param from Starting vector
   * @param to Target vector
   * @return Rotation quaternion
   *
   * I find it quite hard to get quaternions and using starting and finish
   * vectors are more intuitive
   */
  static Quaternion from_vectors(const Vector3 &from, const Vector3 &to) {
    Vector3 f = from.normalized();
    Vector3 t = to.normalized();
    Vector3 crossProd = f.cross(t);
    float dotProd = f.dot(t);

    if (dotProd >= 1.0f)
      return Quaternion(); // No rotation needed

    float w = sqrt((f.magnitude() * f.magnitude()) *
                   (t.magnitude() * t.magnitude())) +
              dotProd;
    Quaternion q(w, crossProd.x, crossProd.y, crossProd.z);
    return q.normalized();
  }

  /**
   * @brief Creates a quaternion that makes an object look at a target point.
   *
   * @param position Object's position
   * @param target Target position to look at
   * @param up Up direction (default: (0,1,0))
   * @return Look-at quaternion
   *
   * Basically wrap for from_vectors function
   */
  static Quaternion lookRotation(const Vector3 &direction,
                                 const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));
};
#endif // QUATERNION_HPP
