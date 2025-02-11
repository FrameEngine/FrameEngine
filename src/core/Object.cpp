#include "Object.hpp"
#include "rendering/Renderer.hpp"
#include <iostream>

void Object::render(Renderer &renderer) {
  Shader *shader = renderer.getShader();
  if (!shader)
    return;

  if (!mesh) {
    std::cerr << "Warning: Object has no mesh!\n";
    return;
  }

  Matrix4 modelMatrix = this->transform->get_transformation_matrix();

  shader->setUniformVec3("objectColor", mesh->getColor());
  shader->setUniformMat4("model", modelMatrix);
  mesh->draw();
}
