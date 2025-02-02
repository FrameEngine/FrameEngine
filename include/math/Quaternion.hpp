#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Matrix4.hpp"
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

  // Create a rotation matrix from a quaternion
  Matrix4 toMatrix() const {
    Matrix4 mat;
    float xx = x * x, xy = x * y, xz = x * z, xw = x * w;
    float yy = y * y, yz = y * z, yw = y * w;
    float zz = z * z, zw = z * w;

    mat.m[0][0] = 1 - 2 * (yy + zz);
    mat.m[0][1] = 2 * (xy - zw);
    mat.m[0][2] = 2 * (xz + yw);

    mat.m[1][0] = 2 * (xy + zw);
    mat.m[1][1] = 1 - 2 * (xx + zz);
    mat.m[1][2] = 2 * (yz - xw);

    mat.m[2][0] = 2 * (xz - yw);
    mat.m[2][1] = 2 * (yz + xw);
    mat.m[2][2] = 1 - 2 * (xx + yy);

    return mat;
  };
};
#endif // QUATERNION_HPP
