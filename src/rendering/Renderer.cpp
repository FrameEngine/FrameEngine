#include "rendering/Renderer.hpp"
#include "objects/Object.hpp"
#include <glad/glad.h>

Shader *Renderer::shader = nullptr;
std::vector<Object *> Renderer::renderQueue;
std::vector<PointLight *> Renderer::lights;

// TODO SO far it's hardcoded, replace with actual width and height
Renderer::Renderer(Registry &registry)
    : camera(Camera(registry, 1920.0f / 1080.0f)) {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  shader =
      new Shader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
}

Camera &Renderer::getCamera() { return camera; }

void Renderer::setShader(Shader *newShader) {
  if (shader) {
    delete shader;
  }
  shader = newShader;
}

void Renderer::clear() {
  glClearColor(0.f, 0.f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::submit(Object *obj) {
  renderQueue.push_back(obj); // Store for later rendering
}

void Renderer::submitLight(PointLight *light) { lights.push_back(light); }

void Renderer::render() {
  shader->bind();

  LOG(DEBUG, "Camera Position: %s", camera.transform->position.toString());
  LOG(DEBUG, "View Matrix:\n%s", camera.getViewMatrix().toString());
  LOG(DEBUG, "Projection Matrix \n%s", camera.getProjectionMatrix().toString());

  shader->setUniformMat4("view", camera.getViewMatrix());
  shader->setUniformMat4("projection", camera.getProjectionMatrix());

  shader->setUniformInt("numLights", lights.size());

  for (size_t i = 0; i < lights.size(); ++i) {
    std::string lightIndex = "lights[" + std::to_string(i) + "]";
    shader->setUniformVec3(lightIndex + ".position",
                           lights[i]->transform->position);
    shader->setUniformVec3(lightIndex + ".color", lights[i]->getColor());
    shader->setUniformFloat(lightIndex + ".intensity",
                            lights[i]->getIntensity());
  }

  for (auto *obj : renderQueue) {
    LOG(DEBUG, "Rendering object at %s", obj->transform->position.toString());
    obj->render(*this);
  }

  shader->unbind();
}

void Renderer::clearObjects() { renderQueue.clear(); }

void Renderer::shutdown() {
  delete shader;
  renderQueue.clear();
  lights.clear();
}
