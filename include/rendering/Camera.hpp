/**
 * @file Camera.hpp
 * @brief Defines the Camera class for view and projection.
 *
 * This file defines a simple Camera.
 * The Camera extends Object, so it already has a transform component.
 */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "objects/Object.hpp"

/**
 * @class Camera
 * @brief A camera.
 */
class Camera : public Object {
private:
  float fov;                ///< Field of view (in degrees).
  float aspectRatio;        ///< The viewport's aspect ratio (width / height).
  float nearPlane;          ///< The near clipping plane.
  float farPlane;           ///< The far clipping plane.
  Matrix4 projectionMatrix; ///< The current projection matrix.
  Matrix4 viewMatrix;       ///< The current view matrix.

public:
  /**
   * @brief Creates a Camera with default settings.
   *
   * By default, the camera uses a 45° field of view, a near plane of 0.1, and a
   * far plane of 100. The aspect ratio is provided as a parameter, by default
   * is 16:9.
   *
   * @param registry The registry where the camera is registered. TODO, make it
   * singletone and refer directly
   * @param aspectRatio The aspect ratio (width/height) of the viewport.
   */
  Camera(Registry &registry, float aspectRatio = 16.f / 9.f)
      : Object(registry, nullptr), fov(45.0f), aspectRatio(aspectRatio),
        nearPlane(0.1f), farPlane(100.0f) {
    transform->position = Vector3(0.0f, 0.0f, 0.0f);
    transform->rotation = Quaternion();

    viewMatrix = Matrix4();
  }

  /**
   * @brief Returns the current view matrix.
   *
   * This method updates the view matrix based on the camera's current position
   * and rotation, then returns it.
   *
   * @return The view matrix.
   */
  Matrix4 getViewMatrix();

  /**
   * @brief Updates the view matrix.
   *
   * Recalculates the view matrix using the camera's current transform.
   * Called in Renderer::render().
   */
  void updateViewMatrix();

  /**
   * @brief Returns the current projection matrix.
   *
   * @return The projection matrix.
   */
  Matrix4 getProjectionMatrix() const;

  /**
   * @brief Sets the camera's projection parameters.
   *
   * This updates the field of view, aspect ratio, near plane, and far plane,
   * and recalculates the projection matrix.
   *
   * @param fov Field of view in degrees.
   * @param aspectRatio Viewport aspect ratio.
   * @param nearPlane Near clipping plane distance.
   * @param farPlane Far clipping plane distance.
   */
  void setProjection(float fov, float aspectRatio, float nearPlane,
                     float farPlane);

  /**
   * @brief Gets the direction the camera is facing.
   *
   * Calculates the forward direction from the camera's rotation.
   *
   * @return A unit Vector3 representing the forward direction.
   */
  Vector3 getFrontVector() const;
};

#endif // CAMERA_HPP
