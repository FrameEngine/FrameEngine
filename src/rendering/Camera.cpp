#include "rendering/Camera.hpp"

/**
 * @brief Computes the view matrix based on position and rotation.
 * @return View matrix for transformations
 */
Matrix4 Camera::getViewMatrix() {
  this->updateViewMatrix();
  return viewMatrix;
}

void Camera::updateViewMatrix() {
  Vector3 front = getFrontVector();

  Vector3 worldUp(0, 1, 0);

  if (std::fabs(front.y) > 0.999f) {
    worldUp = Vector3(0, 0, 1);
  }

  Vector3 right = front.cross(worldUp);
  Vector3 up = right.cross(front);
  Vector3 cameraPos = transform->position;

  viewMatrix = Matrix4({
      right.x, up.x, -front.x, 0.0f,                                        //
      right.y, up.y, -front.y, 0.0f,                                        //
      right.z, up.z, -front.z, 0.0f,                                        //
      -right.dot(cameraPos), -up.dot(cameraPos), front.dot(cameraPos), 1.0f //
  });
}

void Camera::setProjection(float fov, float aspectRatio, float nearPlane,
                           float farPlane) {
  this->fov = fov;
  if (aspectRatio <= 0.0f) {
    this->aspectRatio = 16.0f / 9.0f;
  } else {
    this->aspectRatio = aspectRatio;
  }
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;

  projectionMatrix =
      Matrix4::perspective(fov, this->aspectRatio, nearPlane, farPlane);
}

/**
 * @brief Returns the perspective projection matrix.
 * @return Projection matrix
 */
Matrix4 Camera::getProjectionMatrix() const { return projectionMatrix; }

Vector3 Camera::getFrontVector() const {
  Vector3 front = transform->rotation.rotateVector(Vector3(0, 0, 1));

  if (front.magnitude() < 0.0001f) {
    return Vector3(0, 0, 1);
  }

  return front;
}
