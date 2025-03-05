#include "objects/Object.hpp"
#include "Logger.hpp"
#include "rendering/Renderer.hpp"
#include <iostream>

void Object::render(Renderer &renderer) {
  Shader *shader = renderer.getShader();
  if (!shader || !mesh) {
    return;
  }

  Matrix4 modelMatrix = transform->get_transformation_matrix();

  shader->setUniformVec3("objectColor", color);
  shader->setUniformMat4("model", modelMatrix);

  if (wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    mesh->draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    mesh->draw();
  }
}
