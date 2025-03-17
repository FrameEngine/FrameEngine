#ifndef POSTPROCESSING_PIPELINE_HPP
#define POSTPROCESSING_PIPELINE_HPP

#include "PostProcessingEffect.hpp"
#include <vector>

namespace FrameEngine {

class PostProcessingPipeline {
public:
  PostProcessingPipeline(int width, int height);
  ~PostProcessingPipeline();

  void addEffect(PostProcessingEffect *effect);

  GLuint process(GLuint sceneTexture);

  // Resize all effects (if window size changes).
  void resize(int width, int height);

private:
  int width, height;
  std::vector<PostProcessingEffect *> effects;
};

} // namespace FrameEngine

#endif // POSTPROCESSING_PIPELINE_HPP
