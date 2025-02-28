#define CATCH_CONFIG_MAIN

#include "Registry.hpp"
#include "math/Matrix4.hpp"
#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"
#include "rendering/Camera.hpp"

#include "catch2/catch_all.hpp"

class MockRegistry : public Registry {
public:
  struct Transform {
    Vector3 position;
    Quaternion rotation;
  };

  std::unordered_map<EntityID, Transform> transforms;

  Transform *getTransform(EntityID entity) { return &transforms[entity]; }
};

class CameraFixture {
protected:
  MockRegistry registry;
  Camera *camera;

public:
  CameraFixture() {
    camera = new Camera(registry, 16.0f / 9.0f); // Setup
    camera->transform->position = Vector3(0, 0, 0);
  }

  ~CameraFixture() {
    delete camera; // Tear-down
  }
};

TEST_CASE_METHOD(CameraFixture, "Camera should initialize with default values",
                 "[camera][initialization]") {
  Matrix4 expectedViewMatrix = Matrix4({
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      0.0f, 0.0f, -1.0f, 0.0f, //
      0.0f, 0.0f, 0.0f, 1.0f   //
  });

  Matrix4 viewMatrix = camera->getViewMatrix();
  LOG(DEBUG, "\nExpected: \n%s\nGot: \n%s", expectedViewMatrix.toString(),
      viewMatrix.toString());
  REQUIRE(viewMatrix == expectedViewMatrix);
}

TEST_CASE_METHOD(CameraFixture, "Camera looking at origin from (0, 0, -5)",
                 "[camera][viewMatrix]") {

  camera->transform->position = Vector3(0, 0, -5);
  camera->transform->rotation = Quaternion();
  Matrix4 viewMatrix = camera->getViewMatrix();

  Matrix4 expectedViewMatrix = Matrix4({
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      0.0f, 0.0f, -1.0f, 0.0f, //
      0.0f, 0.0f, -5.0f, 1.0f  //
  });

  REQUIRE(viewMatrix == expectedViewMatrix);
}

TEST_CASE_METHOD(CameraFixture, "Camera looking at origin from (1, 2, 3)",
                 "[camera][viewMatrix]") {
  camera->transform->position = Vector3(1, 2, 3);
  Matrix4 viewMatrix = camera->getViewMatrix();

  Matrix4 expectedViewMatrix = Matrix4({
      1.0f, 0.0f, 0.0f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      0.0f, 0.0f, -1.0f, 0.0f, //
      -1.0f, -2.0f, 3.0f, 1.0f //
  });

  LOG(DEBUG, "\nExpected: \n%s\nGot: \n%s", expectedViewMatrix.toString(),
      viewMatrix.toString());
  REQUIRE(viewMatrix == expectedViewMatrix);
}

TEST_CASE_METHOD(CameraFixture, "Camera rotated 90 degrees around Y-axis",
                 "[camera][viewMatrix]") {
  camera->transform->position = Vector3(0, 0, -3);
  camera->transform->rotation =
      Quaternion::from_axis_angle(Vector3(0, 1, 0), -90.0f);
  Matrix4 viewMatrix = camera->getViewMatrix();

  Matrix4 expectedViewMatrix = Matrix4({
      0.0f, 0.0f, -1.0f, 0.0f, //
      0.0f, 1.0f, 0.0f, 0.0f,  //
      -1.0f, 0.0f, 0.0f, 0.0f, //
      -3.0f, 0.0f, 0.0f, 1.0f  //
  });

  LOG(DEBUG, "\nExpected: \n%s\nGot: \n%s", expectedViewMatrix.toString(),
      viewMatrix.toString());
  REQUIRE(viewMatrix == expectedViewMatrix);
}

TEST_CASE_METHOD(CameraFixture,
                 "Camera moved and rotated 45 degrees around Y-axis",
                 "[camera][viewMatrix]") {
  camera->transform->position = Vector3(2, 0, -4);
  camera->transform->rotation =
      Quaternion::from_axis_angle(Vector3(0, 1, 0), -45.0f);
  Matrix4 viewMatrix = camera->getViewMatrix();

  Matrix4 expectedViewMatrix = Matrix4({
      0.7071f, 0.0f, -0.7071f, 0.0f,  //
      0.0f, 1.0f, 0.0f, 0.0f,         //
      -0.7071f, 0.0f, -0.7071f, 0.0f, //
      -4.2426f, -0.0f, -1.4142f, 1.0f //
  });

  LOG(DEBUG, "\nExpected: \n%s\nGot: \n%s", expectedViewMatrix.toString(),
      viewMatrix.toString());
  REQUIRE(viewMatrix == expectedViewMatrix);
}

TEST_CASE_METHOD(CameraFixture, "Camera should correctly track a moving object",
                 "[camera][tracking]") {
  Vector3 objectPos(0, 0, 0);
  camera->transform->position = Vector3(0, 1, -5);
  camera->transform->rotation = Quaternion();
  camera->lookAt(objectPos);

  Matrix4 initialViewMatrix = camera->getViewMatrix();

  float radius = 1.0f;
  float angle = 3.14159f / 4.0f;
  objectPos = Vector3(radius * cos(angle), 0, radius * sin(angle));

  camera->lookAt(objectPos);
  Matrix4 updatedViewMatrix = camera->getViewMatrix();

  REQUIRE(initialViewMatrix != updatedViewMatrix);

  Vector3 forwardVector = camera->getFrontVector();
  Vector3 expectedDirection =
      (objectPos - camera->transform->position).normalized() * -1;

  REQUIRE(forwardVector.x == Catch::Approx(expectedDirection.x).margin(0.0001));
  REQUIRE(forwardVector.y == Catch::Approx(expectedDirection.y).margin(0.0001));
  REQUIRE(forwardVector.z == Catch::Approx(expectedDirection.z).margin(0.0001));
}

TEST_CASE_METHOD(
    CameraFixture,
    "Camera projection matrix should remain consistent without changes",
    "[camera][projection]") {
  float fov = 60.0f;
  float aspectRatio = 16.0f / 9.0f;
  float nearClip = 0.1f;
  float farClip = 100.0f;

  camera->setProjection(fov, aspectRatio, nearClip, farClip);
  Matrix4 initialProjectionMatrix = camera->getProjectionMatrix();

  // Check if the same matrix remains consistent on repeated calls
  Matrix4 sameProjectionMatrix = camera->getProjectionMatrix();
  REQUIRE(initialProjectionMatrix == sameProjectionMatrix);
}

TEST_CASE_METHOD(CameraFixture,
                 "Camera projection matrix should update on parameter change",
                 "[camera][projection]") {
  float initialFOV = 60.0f;
  float aspectRatio = 16.0f / 9.0f;
  float nearClip = 0.1f;
  float farClip = 100.0f;

  camera->setProjection(initialFOV, aspectRatio, nearClip, farClip);
  Matrix4 initialProjectionMatrix = camera->getProjectionMatrix();

  // Update FOV and verify projection matrix changes
  float updatedFOV = 90.0f;
  camera->setProjection(updatedFOV, aspectRatio, nearClip, farClip);
  Matrix4 updatedProjectionMatrix = camera->getProjectionMatrix();

  REQUIRE(initialProjectionMatrix != updatedProjectionMatrix);

  // Check specific projection matrix elements (optional)
  REQUIRE(updatedProjectionMatrix.m[0][0] ==
          Catch::Approx(
              1.0f / (aspectRatio * tan(updatedFOV * 0.5f * M_PI / 180.0f))));
}

TEST_CASE_METHOD(CameraFixture,
                 "Camera should handle invalid aspect ratio gracefully",
                 "[camera][projection][error]") {
  float invalidAspectRatio = 0.0f; // Edge case: Invalid aspect ratio
  float fov = 60.0f;
  float nearClip = 0.1f;
  float farClip = 100.0f;

  camera->setProjection(fov, invalidAspectRatio, nearClip, farClip);
  Matrix4 projectionMatrix = camera->getProjectionMatrix();

  // Should handle gracefully by either not updating or defaulting to a valid
  // matrix
  REQUIRE_FALSE(std::isinf(projectionMatrix.m[0][0]));
  REQUIRE_FALSE(std::isnan(projectionMatrix.m[0][0]));
}
