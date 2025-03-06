#include "math/Quaternion.hpp"
#include "math/Matrix4.hpp"

/**
 * @brief Converts this quaternion into a 4x4 rotation matrix.
 *
 * The conversion uses the standard formula for constructing a rotation matrix
 * from a normalized quaternion.
 *
 * @return A Matrix4 representing the same rotation as this quaternion.
 */
Matrix4 Quaternion::toMatrix() const {
  float xx = x * x;
  float yy = y * y;
  float zz = z * z;
  float xy = x * y;
  float xz = x * z;
  float yz = y * z;
  float wx = w * x;
  float wy = w * y;
  float wz = w * z;

  return Matrix4(
      {1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f, //
       2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx), 0.0f, //
       2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy), 0.0f, //
       0.0f, 0.0f, 0.0f, 1.0f});                                          //
}

/**
 * @brief Constructs a quaternion from a given rotation matrix.
 *
 * This function uses the standard method of extracting a quaternion from a
 * rotation matrix. It first computes the trace of the matrix to determine which
 * case to use for extraction. The resulting quaternion is normalized before
 * being returned.
 *
 * TLDR; used formulaes from:
 * https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
 *
 * @param matrix The 4x4 rotation matrix to convert.
 * @return A normalized Quaternion representing the same rotation.
 */
Quaternion Quaternion::fromMatrix(const Matrix4 &matrix) {
  // Calculate the trace of the matrix (sum of diagonal elements).
  // The trace is related to the rotation angle.
  float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];

  Quaternion q;
  if (trace > 0) {
    // When the trace is positive, we use the following method to compute
    // a scale factor 's' that stabilizes the division:
    float s = 0.5f / sqrt(trace + 1.0f);

    // The scalar (real) component of the quaternion is computed as:
    q.w = 0.25f / s;

    // The vector components are computed from the differences of the
    // off-diagonal elements.
    q.x = (matrix.m[2][1] - matrix.m[1][2]) * s;
    q.y = (matrix.m[0][2] - matrix.m[2][0]) * s;
    q.z = (matrix.m[1][0] - matrix.m[0][1]) * s;
  } else {
    // If the trace is non-positive, the matrix diagonal has at least one
    // non-positive element.
    // We choose the largest diagonal element to compute the scale factor 's' to
    // avoid instability.
    if (matrix.m[0][0] > matrix.m[1][1] && matrix.m[0][0] > matrix.m[2][2]) {
      // Case 1: m[0][0] is the largest diagonal element.
      // Compute scale factor:
      float s =
          2.0f * sqrt(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]);

      // Compute quaternion components:
      q.w = (matrix.m[2][1] - matrix.m[1][2]) / s;
      q.x = 0.25f * s;
      q.y = (matrix.m[0][1] + matrix.m[1][0]) / s;
      q.z = (matrix.m[0][2] + matrix.m[2][0]) / s;
    } else if (matrix.m[1][1] > matrix.m[2][2]) {
      // Case 2: m[1][1] is the largest diagonal element.
      // Compute scale factor:
      float s =
          2.0f * sqrt(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]);

      // Compute quaternion components:
      q.w = (matrix.m[0][2] - matrix.m[2][0]) / s;
      q.x = (matrix.m[0][1] + matrix.m[1][0]) / s;
      q.y = 0.25f * s;
      q.z = (matrix.m[1][2] + matrix.m[2][1]) / s;
    } else {
      // Case 3: m[2][2] is the largest diagonal element.
      // Compute scale factor:
      float s =
          2.0f * sqrt(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]);

      // Compute quaternion components:
      q.w = (matrix.m[1][0] - matrix.m[0][1]) / s;
      q.x = (matrix.m[0][2] + matrix.m[2][0]) / s;
      q.y = (matrix.m[1][2] + matrix.m[2][1]) / s;
      q.z = 0.25f * s;
    }
  }

  return q.normalized();
}

/**
 * @brief Generates a "look rotation" quaternion.
 *
 * Constructs a quaternion that rotates an object to face in the specified
 * direction.
 *
 * @param direction The desired forward direction.
 * @param up The up vector to align with (default is (0, 1, 0)).
 * @return A quaternion that represents the rotation required for the look
 * direction.
 */
Quaternion Quaternion::lookRotation(const Vector3 &direction,
                                    const Vector3 &up) {
  Vector3 forward = direction.normalized();

  Vector3 currentUp = up;
  if (fabs(forward.dot(up)) > 0.999f) {
    currentUp = Vector3(0, 0, 1);
  }

  Vector3 right = currentUp.cross(forward).normalized();
  Vector3 correctedUp = forward.cross(right);

  Matrix4 rotationMatrix({
      right.x, correctedUp.x, forward.x, 0.0f, //
      right.y, correctedUp.y, forward.y, 0.0f, //
      right.z, correctedUp.z, forward.z, 0.0f, //
      0.0f, 0.0f, 0.0f, 1.0f                   //
  });

  return Quaternion::fromMatrix(rotationMatrix);
}
