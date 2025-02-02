#ifndef MATRIX4_HPP
#define MATRIX4_HPP

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

  Matrix4 operator*(const Matrix4 &other) const {
    Matrix4 result;

    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = 0.0f;
        for (int i = 0; i < 4; i++) {
          result.m[row][col] += m[row][i] * other.m[i][col];
        }
      }
    }

    return result;
  }

  // Create a translation matrix from a Vector3
  static Matrix4 from_translation(const Vector3 &pos) {
    Matrix4 mat;

    mat.m[3][0] = pos.x;
    mat.m[3][1] = pos.y;
    mat.m[3][2] = pos.z;

    return mat;
  }

  // Create a scaling matrix from a Vector3
  static Matrix4 from_scaling(const Vector3 &s) {
    Matrix4 mat;

    mat.m[0][0] = s.x;
    mat.m[1][1] = s.y;
    mat.m[2][2] = s.z;

    return mat;
  }
};

#endif // MATRIX4_HPP
