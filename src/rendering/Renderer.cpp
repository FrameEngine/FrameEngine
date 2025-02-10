#include "rendering/Renderer.hpp"
#include <glad/glad.h>

void Renderer::initialize() { glEnable(GL_DEPTH_TEST); }

void Renderer::clear() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::present() {
  // Swap buffers (GLFW handles this in Window class)
}

void Renderer::shutdown() {
  // Will add in a future. Cleanup
}
