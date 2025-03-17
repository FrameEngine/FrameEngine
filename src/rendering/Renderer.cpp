#include "rendering/Renderer.hpp"
#include "Window.hpp"
#include "objects/Object.hpp"
#include "rendering/PostProcessingPipeline.hpp"
#include <glad/glad.h>

namespace FrameEngine {

/**
 * @brief Constructs a Renderer.
 *
 * @param window A reference to the Window instance.
 */
Renderer::Renderer(Window &window)
    : window(window), camera(Camera(window.getWidth() /
                                    static_cast<float>(window.getHeight()))),
      sceneFBO(0), sceneTexture(0), postProcessingPipeline(nullptr) {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  initialize();
}

Renderer::~Renderer() {
  glDeleteFramebuffers(1, &sceneFBO);
  glDeleteTextures(1, &sceneTexture);
  if (postProcessingPipeline)
    delete postProcessingPipeline;
}

void Renderer::initialize() {
  // Create offscreen framebuffer for scene rendering.
  glGenFramebuffers(1, &sceneFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

  // Create a color attachment texture.
  glGenTextures(1, &sceneTexture);
  glBindTexture(GL_TEXTURE_2D, sceneTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.getWidth(),
               window.getHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         sceneTexture, 0);

  // Create and attach a renderbuffer for depth testing.
  GLuint rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window.getWidth(),
                        window.getHeight());
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LOG(ERROR, "Scene framebuffer is not complete!");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  postProcessingPipeline =
      new PostProcessingPipeline(window.getWidth(), window.getHeight());
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
  // Render system to ofscreen buffer firstly
  glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
  clear();

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

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // === Apply Post–Processing Pipeline ===
  GLuint finalTexture = postProcessingPipeline->process(sceneTexture);

  // Use a simple screen shader that draws a fullscreen quad.
  static Shader *screenShader = nullptr;
  if (!screenShader) {
    screenShader = new Shader("shaders/quad.vs", "shaders/screen.fs");
  }
  screenShader->bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, finalTexture);
  screenShader->setUniformInt("screenTexture", 0);

  // Render a fullscreen quad.
  static GLuint quadVAO = 0, quadVBO = 0;
  if (quadVAO == 0) {
    float quadVertices[] = {// positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                            0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                            1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
  }
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  screenShader->unbind();
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

void Renderer::resize(int width, int height) {
  glViewport(0, 0, width, height);

  glBindTexture(GL_TEXTURE_2D, sceneTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA,
               GL_FLOAT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (postProcessingPipeline)
    postProcessingPipeline->resize(width, height);
}

} // namespace FrameEngine
