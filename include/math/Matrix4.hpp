/**
 * @file Matrix4.hpp
 * @brief Defines the Matrix4 class for 3D transformation operations.
 */

#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Quaternion.hpp"
#include "Vector3.hpp"
#include <iomanip>
#include <sstream>

namespace FrameEngine {

/**
 * @class Matrix4.
 * @brief A 4x4 matrix class.
 *
 * It provides standard arithmetic operations, matrix multiplication, and
 * methods for generating specific transformation matrices.
 *
 * Matrices are row-major ordered.
 *
 * All angles are provided in degrees, unless specified otherwise.
 */
struct Matrix4 {
  float m[4][4];

  /**
   * @brief Default constructor that initializes the matrix as an identity
   * matrix.
   */
  Matrix4() {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        m[i][j] = (i == j) ? 1.0f : 0.0f;
  }

  /**
   * @brief Constructs a Matrix4 from an initializer list of 16 values.
   *
   * @param values An initializer list containing exactly 16 values.
   * @throws std::invalid_argument if the initializer list does not contain
   * exactly 16 values.
   */
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

  /**
   * @brief Assigns the matrix using an initializer list of 16 floats.
   *
   * @param values The initializer list containing exactly 16 values.
   * @return A reference to this matrix.
   * @throws std::invalid_argument if the initializer list does not contain
   * exactly 16 values.
   */
  Matrix4 &operator=(std::initializer_list<float> values) {
    if (values.size() != 16) {
      throw std::invalid_argument("Matrix4 must be assigned exactly 16 values");
    }
    auto it = values.begin();
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        m[row][col] = *it++;
      }
    }
    return *this;
  }

  /**
   * @brief Provides non-const access to a row of the matrix.
   *
   * @param row The index of the row to access.
   * @return A pointer to the first element of the row.
   */
  float *operator[](int row) { return m[row]; }

  /**
   * @brief Creates a rotation matrix from an axis and angle.
   *
   * Constructs a rotation matrix by generating a quaternion from the given
   * axis and angle, and then converting that quaternion to a matrix.
   *
   * @param axis The axis of rotation.
   * @param angle The rotation angle in degrees.
   * @return The rotation matrix.
   */
  static Matrix4 createRotationMatrix(const Vector3 &axis, float angle) {
    Quaternion q = Quaternion::fromAxisAngle(axis, angle);
    return q.toMatrix();
  }

  /**
   * @brief Transforms a vector using the upper 3x3 submatrix.
   *
   * Applies the matrix transformation to the given vector, ignoring any
   * translation (Vector always the same, so translation doesn't matter).
   *
   * @param v The vector to transform.
   * @return The transformed vector.
   */
  Vector3 transformVector(const Vector3 &v) const {
    return {v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
            v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
            v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]};
  }

  /**
   * @brief Returns a string representation of the matrix.
   *
   * The matrix is formatted using fixed-point notation with four decimal
   * places.
   *
   * @return A formatted string representing the matrix.
   */
  std::string toString() const {
    // TODO Add formating options, like floating point precision, output format
    // etc.
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

  /**
   * @brief Multiplies this matrix with another matrix.
   *
   * @param other The matrix to multiply with.
   * @return The resulting matrix after multiplication.
   */
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

  /**
   * @brief Adds another matrix to this matrix.
   *
   * @param other The matrix to add.
   * @return The sum of the two matrices.
   */
  Matrix4 operator+(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] + other.m[row][col];
      }
    }
    return result;
  }

  /**
   * @brief Subtracts another matrix from this matrix.
   *
   * @param other The matrix to subtract.
   * @return The resulting matrix after subtraction.
   */
  Matrix4 operator-(const Matrix4 &other) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] - other.m[row][col];
      }
    }
    return result;
  }

  /**
   * @brief Multiplies the matrix by a scalar.
   *
   * @param scalar The scalar to multiply each element by.
   * @return The scaled matrix.
   */
  Matrix4 operator*(float scalar) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[row][col] * scalar;
      }
    }
    return result;
  }

  /**
   * @brief Divides the matrix by a scalar.
   *
   * If the scalar is zero, the matrix is returned unchanged.
   *
   * @param scalar The scalar to divide by.
   * @return The resulting matrix.
   */
  Matrix4 operator/(float scalar) const {
    // TODO Throw an error if division by zero
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

  /**
   * @brief Compares this matrix with another.
   *
   * Two matrices are considered equal if the difference between each
   * corresponding element is within a small epsilon value.
   *
   * @param other The matrix to compare against.
   * @return True if the matrices are approximately equal, false otherwise.
   */
  bool operator==(const Matrix4 &other) const {
    // TODO Probably should make epsilon customizable
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

  /**
   * @brief Checks whether this matrix is not equal to another
   * matrix.
   *
   * @param other The matrix to compare against.
   * @return True if the matrices differ by more than the epsilon threshold,
   * false otherwise.
   */
  bool operator!=(const Matrix4 &other) const { return !(*this == other); }

  /**
   * @brief Generates a translation matrix.
   *
   * Constructs a matrix that translates by the specified vector.
   *
   * @param pos The translation vector.
   * @return The resulting translation matrix.
   */
  static Matrix4 createTranslationMatrix(const Vector3 &pos) {
    Matrix4 mat;

    mat.m[3][0] = pos.x;
    mat.m[3][1] = pos.y;
    mat.m[3][2] = pos.z;

    return mat;
  }

  /**
   * @brief Generates a scaling matrix.
   *
   * Constructs a matrix that scales along the x, y, and z axes by the given
   * factors.
   *
   * @param s The scaling factors.
   * @return The resulting scaling matrix.
   */
  static Matrix4 createScalingMatrix(const Vector3 &s) {
    Matrix4 mat;

    mat.m[0][0] = s.x;
    mat.m[1][1] = s.y;
    mat.m[2][2] = s.z;

    return mat;
  }

  /**
   * @brief Computes and returns the transpose of the matrix.
   *
   * @return The transposed matrix.
   */
  Matrix4 getTranspose() const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 4; col++) {
        result.m[row][col] = m[col][row];
      }
    }
    return result;
  }

  /**
   * @brief Constructs a perspective projection matrix.
   *
   * @param fov The field of view in degrees.
   * @param aspect The aspect ratio (width/height).
   * @param near The near clipping plane distance.
   * @param far The far clipping plane distance.
   * @return The perspective projection matrix.
   */
  static Matrix4 createPerspectiveMatrix(float fov, float aspect, float near,
                                         float far) {
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

} // namespace FrameEngine

#endif // MATRIX4_HPP
