#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "math/Vector3.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Texture2D.hpp"

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
  Texture2D *texture;

public:
  BasicMaterial(Shader *shader, const Vector3 &diffuse = Vector3(1, 1, 1),
                const Vector3 &specular = Vector3(1, 1, 1), float power = 32.0f,
                Texture2D *texture = nullptr)
      : Material(shader), diffuseColor(diffuse), specularColor(specular),
        specularPower(power), texture(texture) {}

  void setDiffuseColor(const Vector3 &color) { diffuseColor = color; }
  void setSpecularColor(const Vector3 &color) { specularColor = color; }
  void setSpecularPower(float power) { specularPower = power; }
  void setTexture(Texture2D *tex) { texture = tex; }

  virtual void applyUniforms() const override {
    shader->setUniformVec3("material.diffuseColor", diffuseColor);
    shader->setUniformVec3("material.specularColor", specularColor);
    shader->setUniformFloat("material.specularPower", specularPower);

    if (texture) {
      texture->bind(GL_TEXTURE0);
      shader->setUniformInt("texSampler", 0);
    }
  }
};

} // namespace FrameEngine

#endif // MATERIAL_HPP
