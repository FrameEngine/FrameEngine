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

} // namespace FrameEngine

#endif // MATERIAL_HPP
