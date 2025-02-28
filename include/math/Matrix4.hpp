#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Quaternion.hpp"
#include "Vector3.hpp"
#include <iomanip>
#include <sstream>

struct Matrix4 {
  float m[4][4];

  Matrix4() {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        m[i][j] = (i == j) ? 1.0f : 0.0f; // Identity matrix
  }

  Matrix4(std::initializer_list<double> values) {
    if (values.size() != 16) {
      throw std::invalid_argument(
          "Matrix4 must be initialized with exactly 16 values");
    }

    auto it = values.begin();
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        m[row][col] = *it++;
      }
    }
  }

  float *operator[](int row) { return m[row]; }

  const float *operator[](int row) const { return m[row]; }

  static Matrix4 from_rotation(const Vector3 &axis, float angleDegrees) {
    Quaternion q = Quaternion::from_axis_angle(axis, angleDegrees);
    return q.toMatrix();
  }

  // Apply matrix transformation to a vector
  Vector3 transform(const Vector3 &v) const {
    return {v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
            v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
            v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]};
  }

  std::string toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);

    for (int row = 0; row < 4; row++) {
      oss << "[ ";
      for (int col = 0; col < 4; col++) {
        oss << std::setw(8) << m[row][col] << " ";
      }
      oss << "]\n";
    }

    return oss.str();
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

  Matrix4 operator+(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] + other.m[row][col];
      }
    }
    return result;
  }

  Matrix4 operator-(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] - other.m[row][col];
      }
    }
    return result;
  }

  Matrix4 operator*(float scalar) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] * scalar;
      }
    }
    return result;
  }

  Matrix4 operator/(float scalar) const {
    if (scalar == 0.0f)
      return *this;
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] / scalar;
      }
    }
    return result;
  }

  bool operator==(const Matrix4 &other) const {
    constexpr float EPSILON = 1e-4f;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        float diff = std::fabs(m[row][col] - other.m[row][col]);

        if (diff > EPSILON) {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const Matrix4 &other) const { return !(*this == other); }

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

  // Create a perspective projection matrix
  static Matrix4 perspective(float fov, float aspect, float near, float far) {
    float tanHalfFOV = tan(fov * 0.5f * M_PI / 180.0f);
    Matrix4 mat;
    mat.m[0][0] = 1.0f / (aspect * tanHalfFOV);
    mat.m[1][1] = 1.0f / tanHalfFOV;
    mat.m[2][2] = -(far + near) / (far - near);
    mat.m[2][3] = -1.0f;
    mat.m[3][2] = -(2.0f * far * near) / (far - near);
    mat.m[3][3] = 0.0f;
    return mat;
  }
};

#endif // MATRIX4_HPP
