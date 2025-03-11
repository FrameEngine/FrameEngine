#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "math/Vector3.hpp"
#include "rendering/Shader.hpp"
#include <string>

namespace FrameEngine {

class Material {
private:
  Vector3 diffuseColor;
  Vector3 specularColor;
  float specularPower;

public:
  Material() {};
  ~Material();

  Vector3 getDiffuseColor() { return diffuseColor; };
  Vector3 getSpecularColor() { return specularColor; };
  float getSpecularPower() { return specularPower; };

  void setDiffuseColor(const Vector3 &color) { diffuseColor = color; };
  void setSpecularColor(const Vector3 &color) { specularColor = color; };
  void setSpecularPower(float power) { specularPower = power; };
};

} // namespace FrameEngine

#endif // MATERIAL_HPP
