#include "rendering/PostProcessingPipeline.hpp"

namespace FrameEngine {

PostProcessingPipeline::PostProcessingPipeline(int width, int height)
    : width(width), height(height) {}

PostProcessingPipeline::~PostProcessingPipeline() {
  for (auto effect : effects) {
    delete effect;
  }
}

void PostProcessingPipeline::addEffect(PostProcessingEffect *effect) {
  effect->init(width, height);
  effects.push_back(effect);
}

GLuint PostProcessingPipeline::process(GLuint sceneTexture) {
  GLuint inputTexture = sceneTexture;
  for (auto effect : effects) {
    effect->apply(inputTexture);
    inputTexture = effect->getOutputTexture();
  }
  return inputTexture;
}

void PostProcessingPipeline::resize(int width, int height) {
  this->width = width;
  this->height = height;
  for (auto effect : effects) {
    effect->resize(width, height);
  }
}

} // namespace FrameEngine
