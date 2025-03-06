#include "rendering/Camera.hpp"

/**
 * @brief Returns the current view matrix.
 *
 * This function updates the view matrix based on the camera's current position
 * and rotation, then returns the updated view matrix.
 *
 * @return The view matrix.
 */
Matrix4 Camera::getViewMatrix() {
  this->updateViewMatrix();
  return viewMatrix;
}

/**
 * @brief Updates the view matrix based on the camera's transform.
 *
 * This method computes the view matrix using the camera's position and
 * rotation. It calculates the camera's forward vector and then
 * creates the view matrix.
 */
void Camera::updateViewMatrix() {
  Vector3 front = getFrontVector();
  Vector3 worldUp(0, 1, 0);

  // Avoid Gimbal lock
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

/**
 * @brief Sets the camera's projection parameters.
 *
 * Updates the field of view, aspect ratio, near plane, and far plane, then
 * calculates a new perspective projection matrix.
 *
 * @param fov Field of view in degrees.
 * @param aspectRatio The aspect ratio (width/height) of the viewport.
 * @param nearPlane The near clipping distance.
 * @param farPlane The far clipping distance.
 */
void Camera::setProjection(float fov, float aspectRatio, float nearPlane,
                           float farPlane) {
  this->fov = fov;
  // Use a default aspect ratio if an invalid value is provided.
  this->aspectRatio = (aspectRatio <= 0.0f) ? (16.0f / 9.0f) : aspectRatio;

  this->nearPlane = nearPlane;
  this->farPlane = farPlane;

  projectionMatrix = Matrix4::createPerspectiveMatrix(fov, this->aspectRatio,
                                                      nearPlane, farPlane);
}

/**
 * @brief Returns the current projection matrix.
 *
 * @return The perspective projection matrix.
 */
Matrix4 Camera::getProjectionMatrix() const { return projectionMatrix; }

/**
 * @brief Calculates and returns the camera's front (forward) vector.
 *
 * This method rotates the default forward vector (0, 0, 1) using the camera's
 * current rotation. If the resulting vector is nearly zero, it returns the
 * default forward vector.
 *
 * @return The forward direction of the camera.
 */
Vector3 Camera::getFrontVector() const {
  Vector3 front = transform->rotation.rotateVector(Vector3(0, 0, 1));

  // If the calculated front vector is very small, default to (0, 0, 1).
  if (front.magnitude() < 0.0001f) {
    return Vector3(0, 0, 1);
  }

  return front;
}
