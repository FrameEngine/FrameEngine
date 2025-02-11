#include "rendering/Renderer.hpp"
#include "Object.hpp"
#include <glad/glad.h>
#include <iostream>

Shader *Renderer::shader = nullptr;
std::vector<Object *> Renderer::renderQueue;

// TODO SO far it's hardcoded, replace with actual width and height
Renderer::Renderer() : camera(Camera(1920.0f / 1080.0f)) {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  shader =
      new Shader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
}

Camera &Renderer::getCamera() { return camera; }

void Renderer::setShader(Shader *shader) { this->shader = shader; }

void Renderer::clear() {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::submit(Object *obj) {
  renderQueue.push_back(obj); // Store for later rendering
}

void Renderer::render() {
  shader->bind();

  shader->setUniformMat4("view", camera.getViewMatrix());
  shader->setUniformMat4("projection", camera.getProjectionMatrix());

  for (auto *obj : renderQueue) {
    obj->render(*this);
  }

  shader->unbind();
}

void Renderer::clearObjects() { renderQueue.clear(); }

void Renderer::shutdown() { delete shader; }
