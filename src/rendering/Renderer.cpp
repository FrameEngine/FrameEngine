#include "rendering/Renderer.hpp"
#include <glad/glad.h>
#include <iostream>

Shader *Renderer::shader = nullptr;
std::vector<Mesh *> Renderer::renderQueue;

void Renderer::initialize() {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  shader =
      new Shader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
}

void Renderer::clear() {
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::submit(Mesh *mesh) {
  renderQueue.push_back(mesh); // Store for later rendering
}

void Renderer::render() {
  shader->bind();

  for (auto *mesh : renderQueue) {
    shader->setUniformVec3("objectColor", mesh->getColor());
    mesh->draw();
  }

  shader->unbind();
}

void Renderer::shutdown() { delete shader; }
