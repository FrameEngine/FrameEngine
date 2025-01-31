#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3.hpp"
#include <cmath>

struct Quaternion {
  float w, x, y, z;

  Quaternion() : w(1), x(0), y(0), z(0) {}

  Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

  Quaternion normalize() const {
    float mag = std::sqrt(w * w + x * x + y * y + z * z);
    return {w / mag, x / mag, y / mag, z / mag};
  }

  static Quaternion from_axis_angle(const Vector3 &axis, float angle) {
    float rad = angle * M_PI / 180.0f;
    float sinHalf = std::sin(rad / 2);
    return Quaternion(std::cos(rad / 2), axis.x * sinHalf, axis.y * sinHalf,
                      axis.z * sinHalf)
        .normalize();
  }

  Quaternion operator*(const Quaternion &q) const {
    return {w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w};
  }

  Vector3 rotate(const Vector3 &v) const {
    Quaternion vecQuat(0, v.x, v.y, v.z);
    Quaternion invQuat(w, -x, -y, -z);
    Quaternion result = (*this) * vecQuat * invQuat;
    return {result.x, result.y, result.z};
  }
};

#endif // QUATERNION_HPP
