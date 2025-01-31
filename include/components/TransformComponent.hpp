#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"

struct TransformComponent {
  Vector3 position;
  Quaternion rotation;
  Vector3 scale = {1, 1, 1};
};

#endif // TRANSFORM_COMPONENT_HPP
