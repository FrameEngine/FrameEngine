#include "objects/Object.hpp"
#include "Logger.hpp"
#include "components/MaterialComponent.hpp"
#include "math/Vector3.hpp"
#include "rendering/Renderer.hpp"

namespace FrameEngine {

void Object::setMesh(Mesh *mesh, bool wireframe) {
  if (!get_component<MeshComponent>()) {
    add_component<MeshComponent>(MeshComponent(mesh));
  } else {
    get_component<MeshComponent>()->mesh = mesh;
  }
  get_component<MeshComponent>()->wireframe = wireframe;
}

void Object::setMaterial(Material *material) {
  if (!get_component<MaterialComponent>()) {
    add_component<MaterialComponent>(MaterialComponent(material));
  } else {
    get_component<MaterialComponent>()->material = material;
  }
}

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
  if (!shader || !meshComponent) {
    return;
  }

  shader->bind();

  if (!materialComponent) {
    shader->setUniformVec3("material.diffuseColor", Vector3(1, 1, 1));
    shader->setUniformVec3("material.specularColor", Vector3(1.0f, 1.0f, 1.0f));
    shader->setUniformFloat("material.specularPower", 64.0f);
  } else {
    shader->setUniformVec3("material.diffuseColor",
                           materialComponent->material->getDiffuseColor());
    shader->setUniformVec3("material.specularColor",
                           materialComponent->material->getSpecularColor());
    shader->setUniformFloat("material.specularPower",
                            materialComponent->material->getSpecularPower());
  }

  Matrix4 modelMatrix = transform->get_transformation_matrix();
  shader->setUniformMat4("model", modelMatrix);

  if (meshComponent->wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    meshComponent->mesh->draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    meshComponent->mesh->draw();
  }

  shader->unbind();
}

} // namespace FrameEngine
