/**
 * @file Vector3.hpp
 * @brief Defines the Vector3 class.
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <sstream>
#include <string>

class Vector3 {
public:
  double x, y, z;

  /**
   * @brief Constructs a Vector3 with specified components.
   *
   * @param x The x component (default is 0).
   * @param y The y component (default is 0).
   * @param z The z component (default is 0).
   */
  Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  /**
   * @brief Adds this vector to another vector.
   *
   * @param v The vector to add.
   * @return A new vector that is the sum of this vector and v.
   */
  Vector3 operator+(const Vector3 &v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
  };

  /**
   * @brief Subtracts another vector from this vector.
   *
   * @param v The vector to subtract.
   * @return A new vector that is the difference between this vector and v.
   */
  Vector3 operator-(const Vector3 &v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
  };

  /**
   * @brief Multiplies this vector by a scalar.
   *
   * @param scalar The scalar multiplier.
   * @return A new vector with each component scaled by scalar.
   */
  Vector3 operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  };

  /**
   * @brief Multiplies this vector by a scalar in-place.
   *
   * e.g `Vector3*=4.f;`.
   *
   * @param scalar The scalar multiplier.
   * @return A reference to this vector after scaling.
   */
  Vector3 &operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  /**
   * @brief Divides this vector by a scalar.
   *
   * @param scalar The scalar divisor.
   * @return A new vector with each component divided by scalar.
   */
  Vector3 operator/(double scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  };

  /**
   * @brief Compares two vectors for equality.
   *
   * Two vectors are considered equal if their corresponding components are
   * exactly equal.
   *
   * @param v The vector to compare.
   * @return True if the vectors are equal; false otherwise.
   */
  bool operator==(const Vector3 &v) const {
    return (x == v.x) && (y == v.y) && (z == v.z);
  };

  /**
   * @brief Adds another vector to this vector in-place.
   *
   * e.g `Vector3+=anotherVector3`.
   *
   * @param v The vector to add.
   * @return A reference to this vector after addition.
   */
  Vector3 &operator+=(const Vector3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  /**
   * @brief Computes the magnitude of this vector.
   *
   * @return The Euclidean norm of the vector.
   */
  double magnitude() const { return std::sqrt(x * x + y * y + z * z); }

  /**
   * @brief Returns a normalized (unit length) version of this vector.
   *
   * If the vector is zero, a zero vector is returned.
   *
   * @return A normalized vector.
   */
  Vector3 normalized() const {
    double length = magnitude();
    return (length == 0) ? Vector3(0, 0, 0) : *this / magnitude();
  }

  /**
   * @brief Computes the dot product between this vector and another.
   *
   * @param other The other vector.
   * @return The dot product.
   */
  float dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  /**
   * @brief Computes the cross product between this vector and another.
   *
   * @param other The other vector.
   * @return A new vector that is the cross product.
   */
  Vector3 cross(const Vector3 &other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
  }

  /**
   * @brief Returns a string representation of the vector.
   *
   * The format is: "(x, y, z)".
   *
   * @return A string representing the vector.
   */
  std::string toString() const {
    std::ostringstream ss;
    ss << "(" << x << ", " << y << ", " << z << ")";
    return ss.str();
  }
};

/**
 * @brief Multiplies a scalar with a Vector3 (scalar on the left).
 *
 * @param scalar The scalar multiplier.
 * @param v The vector.
 * @return The scaled vector.
 */
inline Vector3 operator*(double scalar, const Vector3 &v) { return v * scalar; }

#endif // !VECTOR3_H
