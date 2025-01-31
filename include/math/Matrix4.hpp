#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Quaternion.hpp"
#include "Vector3.hpp"

struct Matrix4 {
  float m[4][4];

  Matrix4() {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        m[i][j] = (i == j) ? 1.0f : 0.0f; // Identity matrix
  }

  // Apply matrix transformation to a vector
  Vector3 transform(const Vector3 &v) const {
    return {v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
            v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
            v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]};
  }

  // Create a rotation matrix from a quaternion
  // Stole formulae from here: https://www.3dgep.com/understanding-quaternions/
  static Matrix4 from_quaternion(const Quaternion &q) {
    Matrix4 mat;
    float xx = q.x * q.x, xy = q.x * q.y, xz = q.x * q.z, xw = q.x * q.w;
    float yy = q.y * q.y, yz = q.y * q.z, yw = q.y * q.w;
    float zz = q.z * q.z, zw = q.z * q.w;

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
  }
};

#endif // MATRIX4_HPP
