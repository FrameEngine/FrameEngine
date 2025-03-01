#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "objects/Object.hpp"

class Camera : public Object {
private:
  float fov, aspectRatio, nearPlane, farPlane;
  Matrix4 projectionMatrix;
  Matrix4 viewMatrix;

public:
  Camera(Registry &registry, float aspectRatio = 16.f / 9.f)
      : Object(registry, nullptr), fov(45.0f), aspectRatio(aspectRatio),
        nearPlane(0.1f), farPlane(100.0f) {
    transform->position = Vector3(0.0f, 0.0f, 0.0f);
    transform->rotation = Quaternion();

    viewMatrix = Matrix4();
  }

  Matrix4 getViewMatrix();
  void updateViewMatrix();

  Matrix4 getProjectionMatrix() const;
  void setProjection(float fov, float aspectRatio, float nearPlane,
                     float farPlane);

  Vector3 getFrontVector() const;
};

#endif // CAMERA_HPP
