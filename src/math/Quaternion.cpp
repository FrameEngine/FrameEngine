#include "math/Quaternion.hpp"
#include "math/Matrix4.hpp"

Matrix4 Quaternion::toMatrix() const {
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

Quaternion Quaternion::fromMatrix(const Matrix4 &matrix) {
  float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];

  Quaternion q;
  if (trace > 0) {
    float s = 0.5f / sqrt(trace + 1.0f);
    q.w = 0.25f / s;
    q.x = (matrix.m[2][1] - matrix.m[1][2]) * s;
    q.y = (matrix.m[0][2] - matrix.m[2][0]) * s;
    q.z = (matrix.m[1][0] - matrix.m[0][1]) * s;
  } else {
    if (matrix.m[0][0] > matrix.m[1][1] && matrix.m[0][0] > matrix.m[2][2]) {
      float s =
          2.0f * sqrt(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]);
      q.w = (matrix.m[2][1] - matrix.m[1][2]) / s;
      q.x = 0.25f * s;
      q.y = (matrix.m[0][1] + matrix.m[1][0]) / s;
      q.z = (matrix.m[0][2] + matrix.m[2][0]) / s;
    } else if (matrix.m[1][1] > matrix.m[2][2]) {
      float s =
          2.0f * sqrt(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]);
      q.w = (matrix.m[0][2] - matrix.m[2][0]) / s;
      q.x = (matrix.m[0][1] + matrix.m[1][0]) / s;
      q.y = 0.25f * s;
      q.z = (matrix.m[1][2] + matrix.m[2][1]) / s;
    } else {
      float s =
          2.0f * sqrt(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]);
      q.w = (matrix.m[1][0] - matrix.m[0][1]) / s;
      q.x = (matrix.m[0][2] + matrix.m[2][0]) / s;
      q.y = (matrix.m[1][2] + matrix.m[2][1]) / s;
      q.z = 0.25f * s;
    }
  }

  return q.normalized();
}

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
