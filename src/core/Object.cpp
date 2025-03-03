#include "objects/Object.hpp"
#include "Logger.hpp"
#include "rendering/Renderer.hpp"
#include <iostream>

void Object::render(Renderer &renderer) {
  Shader *shader = renderer.getShader();
  if (!shader)
    return;

  if (!mesh) {
    return;
  }

  Matrix4 modelMatrix = transform->get_transformation_matrix();

  LOG(DEBUG, "modelMatrix: \n%s", modelMatrix.toString());

  shader->setUniformVec3("objectColor", color);
  shader->setUniformMat4("model", modelMatrix);
  mesh->draw();
}
