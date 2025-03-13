#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "math/Vector3.hpp"
#include "rendering/Shader.hpp"

namespace FrameEngine {

class Material {
protected:
  Shader *shader;

public:
  Material(Shader *shader) : shader(shader) {}
  virtual ~Material() {}

  Shader *getShader() const { return shader; }

  virtual void applyUniforms() const = 0;
};

class BasicMaterial : public Material {
private:
  Vector3 diffuseColor;
  Vector3 specularColor;
  float specularPower;

public:
  BasicMaterial(Shader *shader, const Vector3 &diffuse = Vector3(1, 1, 1),
                const Vector3 &specular = Vector3(1, 1, 1), float power = 32.0f)
      : Material(shader), diffuseColor(diffuse), specularColor(specular),
        specularPower(power) {}

  void setDiffuseColor(const Vector3 &color) { diffuseColor = color; }
  void setSpecularColor(const Vector3 &color) { specularColor = color; }
  void setSpecularPower(float power) { specularPower = power; }

  virtual void applyUniforms() const override {
    shader->setUniformVec3("material.diffuseColor", diffuseColor);
    shader->setUniformVec3("material.specularColor", specularColor);
    shader->setUniformFloat("material.specularPower", specularPower);
  }
};

} // namespace FrameEngine

#endif // MATERIAL_HPP
