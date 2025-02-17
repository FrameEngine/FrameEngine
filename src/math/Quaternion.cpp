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

  return q.normalize();
}

Quaternion Quaternion::lookAt(const Vector3 &direction, const Vector3 &up) {
  Vector3 forward = direction.normalized();

  Vector3 right = forward.cross(up).normalized() *
                  -1; // TODO Have no idea why does it work XD
  Vector3 newUp = right.cross(forward).normalized();

  // Create rotation matrix
  Matrix4 rotationMatrix;
  rotationMatrix.m[0][0] = right.x;
  rotationMatrix.m[0][1] = right.y;
  rotationMatrix.m[0][2] = right.z;
  rotationMatrix.m[1][0] = newUp.x;
  rotationMatrix.m[1][1] = newUp.y;
  rotationMatrix.m[1][2] = newUp.z;
  rotationMatrix.m[2][0] = -forward.x;
  rotationMatrix.m[2][1] = -forward.y;
  rotationMatrix.m[2][2] = -forward.z;

  return Quaternion::fromMatrix(rotationMatrix);
}
