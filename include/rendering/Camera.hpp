#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include "objects/Object.hpp"

class Camera : public Object {
private:
  float fov, aspectRatio, nearPlane, farPlane;
  Matrix4 projectionMatrix;

public:
  Camera(Registry &registry, float aspectRatio)
      : Object(registry, nullptr), fov(45.0f), aspectRatio(aspectRatio),
        nearPlane(0.1f), farPlane(100.0f) {
    transform->position = Vector3(0.0f, 1.0f, -2.5f);
    transform->rotation = Quaternion();
  }

  Matrix4 getViewMatrix() const;
  Matrix4 getProjectionMatrix() const;

  Vector3 getFrontVector() const;

  void setProjection(float fov, float aspectRatio, float nearPlane,
                     float farPlane);
};

#endif // CAMERA_HPP
