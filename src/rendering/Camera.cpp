#include "rendering/Camera.hpp"

Matrix4 Camera::getViewMatrix() const {
  return Matrix4::lookAt(position, position + front, up);
}

void Camera::setPosition(const Vector3 &newPosition) { position = newPosition; }

/**
 * @brief Rotates the camera to look at a target point.
 * @param target Target position
 */
void Camera::lookAt(const Vector3 &target) {
  front = (target - position).normalized();
}
