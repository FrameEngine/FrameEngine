#include "rendering/Renderer.hpp"
#include <iostream>

IRenderer *Renderer::renderer = nullptr;

void Renderer::set_renderer(IRenderer *newRenderer) { renderer = newRenderer; }

void Renderer::init() {
  if (renderer) {
    renderer->initialize();
  } else {
    std::cerr << "Error: No renderer set\n";
  }
}

void Renderer::clear() {
  if (renderer)
    renderer->clearScreen();
}

void Renderer::draw_mesh(Mesh &mesh, const Matrix4 &transformMatrix) {
  if (renderer) {
    renderer->drawMesh(mesh, transformMatrix);
  }
}

void Renderer::present() {
  if (renderer)
    renderer->swapBuffers();
}

void Renderer::shutdown() {
  if (renderer)
    renderer->shutdown();
}
