#ifndef POSTPROCESSING_EFFECT_HPP
#define POSTPROCESSING_EFFECT_HPP

#include <glad/glad.h>

namespace FrameEngine {

class PostProcessingEffect {
public:
  virtual ~PostProcessingEffect() {}

  virtual void init(int width, int height) = 0;

  virtual void resize(int width, int height) = 0;

  virtual void apply(GLuint inputTexture) = 0;

  virtual GLuint getOutputTexture() const = 0;
};

} // namespace FrameEngine

#endif // POSTPROCESSING_EFFECT_HPP
