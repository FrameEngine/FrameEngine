#include "rendering/Renderer.hpp"
#include "Window.hpp"
#include "objects/Object.hpp"
#include <glad/glad.h>

namespace FrameEngine {

/**
 * @brief Constructs a Renderer.
 *
 * @param window A reference to the Window instance.
 */
Renderer::Renderer(Window &window)
    : window(window), camera(Camera(window.getWidth() / window.getHeight())) {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
}

/**
 * @brief Returns a reference to the camera.
 *
 * @return A reference to the camera.
 */
Camera &Renderer::getCamera() { return camera; }

/**
 * @brief Renders all submitted entities using batched material groups.
 *
 * This method queries the global Registry for all entities that contain the
 * required rendering components (TransformComponent, MeshComponent, and
 * MaterialComponent). It groups these entities by their associated material,
 * then updates the global lighting data from the ECS using the LightingSystem.
 *
 * (The lighting UBO is bound to a fixed binding point so that all
 * shaders can access the updated lighting data).
 *
 * For each material batch, the method binds the material's shader once and sets
 * the common uniforms (view matrix, projection matrix, and camera position)
 * from the provided Camera. The material-specific uniforms are then applied via
 * the material's applyUniforms() method. Finally, for every entity in the
 * batch, only the per-object uniform (the model matrix) is updated before
 * issuing the draw call.
 *
 * @note This method should be called once per frame, typically after simulation
 * updates and before swapping the window buffers.
 */
void Renderer::render() {
  Registry &registry = Registry::getInstance();

  LightingSystem::update();

  Matrix4 viewMatrix = camera.getViewMatrix();
  Matrix4 projectionMatrix = camera.getProjectionMatrix();
  Vector3 cameraPos = camera.transform->position;

  // Build batches by materail-type. Each batch uses the same shader, so all
  // of the objects with the same material can be rendered in one call to GPU
  std::unordered_map<Material *, std::vector<EntityID>> batches;
  auto entities = registry.get_entities_with_component<MeshComponent>();
  for (EntityID entity : entities) {
    if (!registry.has_component<TransformComponent>(entity) ||
        !registry.has_component<MaterialComponent>(entity))
      continue;

    auto &matComp = registry.get_component<MaterialComponent>(entity);
    Material *mat = matComp.material;
    if (!mat)
      continue;
    batches[mat].push_back(entity);
  }

  // Process each batch.
  for (auto &pair : batches) {
    Material *material = pair.first;
    Shader *shader = material->getShader();
    if (!shader)
      continue;

    shader->bind();

    shader->setUniformVec3("ambientColor", Vector3(0.2f, 0.2f, 0.2f));

    // Probably should move to UBO as well
    shader->setUniformMat4("view", viewMatrix);
    shader->setUniformMat4("projection", projectionMatrix);
    shader->setUniformVec3("viewPos", cameraPos);

    material->applyUniforms();

    // Render each entity in the batch.
    for (EntityID entity : pair.second) {
      auto &transform = registry.get_component<TransformComponent>(entity);
      Matrix4 modelMatrix = transform.get_transformation_matrix();
      shader->setUniformMat4("model", modelMatrix);

      auto &meshComp = registry.get_component<MeshComponent>(entity);
      if (meshComp.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        meshComp.mesh->draw();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else {
        meshComp.mesh->draw();
      }
    }

    shader->unbind();
  }
}

/**
 * @brief Clears the screen before rendering a new frame.
 *
 * This should be called at the beginning of each frame to clear
 * the previous frame's contents.
 */
/// TODO Replace hardcoded color with variable
void Renderer::clear() {
  glClearColor(0.f, 0.f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace FrameEngine
