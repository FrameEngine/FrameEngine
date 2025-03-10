#include "objects/Object.hpp"
#include "Logger.hpp"
#include "rendering/Renderer.hpp"

/**
 * @brief Renders the object using the current shader from the renderer.
 *
 * This method retrieves the active shader from the provided Renderer instance,
 * computes the model transformation matrix, and updates the shader uniforms
 * (object color and model matrix). If the object is flagged for wireframe
 * rendering, the OpenGL polygon mode is temporarily set to GL_LINE before
 * drawing the mesh, then restored to GL_FILL afterward.
 *
 * @param renderer The Renderer instance responsible for drawing.
 */
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
