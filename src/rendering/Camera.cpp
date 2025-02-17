#include "rendering/Camera.hpp"

/**
 * @brief Computes the view matrix based on position and rotation.
 * @return View matrix for transformations
 */
Matrix4 Camera::getViewMatrix() const {

  Vector3 front = getFrontVector();

  Vector3 right = Vector3(0, 1, 0).cross(front).normalized();
  Vector3 up = front.cross(right).normalized();

  return Matrix4::lookAt(transform->position, transform->position + front, up);
}

/**
 * @brief Returns the perspective projection matrix.
 * @return Projection matrix
 */
Matrix4 Camera::getProjectionMatrix() const {
  return Matrix4::perspective(fov, aspectRatio, nearPlane, farPlane);
}

Vector3 Camera::getFrontVector() const {
  Vector3 front = transform->rotation.rotateVector(Vector3(0, 0, -1));

  if (front.magnitude() < 0.0001f) {
    return Vector3(0, 0, -1);
  }

  return front.normalized();
}
