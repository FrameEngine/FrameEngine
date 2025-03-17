#include "rendering/Material.hpp"

namespace FrameEngine {
class BasicMaterial : public Material {
private:
  Vector3 diffuseColor;
  Vector3 specularColor;
  float specularPower;

  bool emissiveEnabled;
  Vector3 emissiveColor;

  Texture2D *defaultTexture;
  Texture2D *texture;

public:
  BasicMaterial(Shader *shader,                               //
                const Vector3 &diffuse = Vector3(1, 1, 1),    //
                const Vector3 &specular = Vector3(1, 1, 1),   //
                float power = 32.0f,                          //
                Texture2D *texture = nullptr,                 //
                bool emissive = false,                        //
                const Vector3 &emissiveCol = Vector3(0, 0, 0) //
                )
      : Material(shader), diffuseColor(diffuse), specularColor(specular),
        specularPower(power), texture(texture), emissiveEnabled(emissive),
        emissiveColor(emissiveCol) {
    defaultTexture = new Texture2D();
    defaultTexture->createDefaultWhiteTexture();
  }

  void setDiffuseColor(const Vector3 &color) { diffuseColor = color; }
  void setSpecularColor(const Vector3 &color) { specularColor = color; }
  void setSpecularPower(float power) { specularPower = power; }

  void setTexture(Texture2D *tex) { texture = tex; }

  bool isEmissiveEnabled() const { return emissiveEnabled; }
  void setEmissiveEnabled(bool flag) { emissiveEnabled = flag; }
  void setEmissiveColor(const Vector3 &col) { emissiveColor = col; }

  virtual void applyUniforms() const override {
    shader->setUniformVec3("material.diffuseColor", diffuseColor);
    shader->setUniformVec3("material.specularColor", specularColor);
    shader->setUniformFloat("material.specularPower", specularPower);

    shader->setUniformInt("material.emissiveEnabled", emissiveEnabled ? 1 : 0);
    shader->setUniformVec3("material.emissiveColor", emissiveColor);

    if (texture) {
      texture->bind(GL_TEXTURE0);
      shader->setUniformInt("texSampler", 0);
    } else {
      defaultTexture->bind(GL_TEXTURE0);
    }
  }
};

} // namespace FrameEngine
