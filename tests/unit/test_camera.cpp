#define CATCH_CONFIG_MAIN
#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"
#include "rendering/Camera.hpp"

#include "catch2/catch_all.hpp"

TEST_CASE("Camera View Matrix Updates on Movement", "[camera]") {
  Camera camera;
  camera.setPosition(Vector3(0, 0, 5));

  Matrix4 initialViewMatrix = camera.getViewMatrix();

  // Move the camera to a new position
  camera.setPosition(Vector3(1, 2, 3));
  Matrix4 updatedViewMatrix = camera.getViewMatrix();

  REQUIRE(initialViewMatrix != updatedViewMatrix);
}

TEST_CASE("Camera Tracks Moving Object", "[camera]") {
  Camera camera;
  Vector3 objectPos(0, 0, 0);
  camera.lookAt(objectPos);

  Matrix4 initialViewMatrix = camera.getViewMatrix();

  // Move the object in a circular path
  float radius = 1.0f;
  float angle = 3.14159f / 4.0f; // 45 degrees
  objectPos = Vector3(radius * cos(angle), 0, radius * sin(angle));

  camera.lookAt(objectPos);
  Matrix4 updatedViewMatrix = camera.getViewMatrix();

  REQUIRE(initialViewMatrix != updatedViewMatrix);
}

TEST_CASE("Projection Matrix Consistency", "[camera]") {
  Camera camera;
  float fov = 60.0f;
  float aspectRatio = 16.0f / 9.0f;
  float nearClip = 0.1f;
  float farClip = 100.0f;

  camera.setProjection(fov, aspectRatio, nearClip, farClip);
  Matrix4 initialProjectionMatrix = camera.getProjectionMatrix();

  // Ensure projection matrix stays the same
  Matrix4 sameProjectionMatrix = camera.getProjectionMatrix();
  REQUIRE(initialProjectionMatrix == sameProjectionMatrix);

  // Change FOV, should change projection matrix
  camera.setProjection(90.0f, aspectRatio, nearClip, farClip);
  Matrix4 updatedProjectionMatrix = camera.getProjectionMatrix();

  REQUIRE(initialProjectionMatrix != updatedProjectionMatrix);
}
