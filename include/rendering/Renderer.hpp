/**
 * @file Renderer.hpp
 * @brief Manages the rendering pipeline and object submission.
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Window.hpp"
#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/LightingSystem.hpp"
#include "rendering/PostProcessingPipeline.hpp"
#include "rendering/Shader.hpp"
#include <vector>

namespace FrameEngine {

/**
 * @class Renderer
 * @brief Handles the drawing of scene objects and lights.
 *
 * The Renderer class keeps a list of objects and point lights to render.
 */
class Renderer {
private:
  Camera camera;
  Window &window;

  GLuint sceneFBO;
  GLuint sceneTexture;

  PostProcessingPipeline *postProcessingPipeline;

public:
  /**
   * @brief Constructs a Renderer and sets up the camera.
   *
   * @param window A reference to the Window instance.
   */
  Renderer(Window &window);
  ~Renderer();

  /**
   * @brief Initializes the rendering system.
   *
   * This method must be called before any other rendering function.
   */
  void initialize();

  /**
   * @brief Clears the screen before rendering a new frame.
   *
   * This should be called at the beginning of each frame to clear
   * the previous frame's contents.
   */
  static void clear();

  /**
   * @brief Renders all submitted entities using batched material groups.
   *
   * This method queries the global Registry for all entities that contain the
   * required rendering components (TransformComponent, MeshComponent, and
   * MaterialComponent). It groups these entities by their associated material,
   * then updates the global lighting data from the ECS using the
   * LightingSystem.
   *
   * (The lighting UBO is bound to a fixed binding point so that all
   * shaders can access the updated lighting data).
   *
   * For each material batch, the method binds the material's shader once and
   * sets the common uniforms (view matrix, projection matrix, and camera
   * position) from the provided Camera. The material-specific uniforms are then
   * applied via the material's applyUniforms() method. Finally, for every
   * entity in the batch, only the per-object uniform (the model matrix) is
   * updated before issuing the draw call.
   *
   * @note This method should be called once per frame, typically after
   * simulation updates and before swapping the window buffers.
   */
  void render();

  /**
   * @brief Shuts down the rendering system and releases resources.
   *
   * This method should be called when the application is closing
   * to properly clean up the rendering context.
   */
  static void shutdown();

  /**
   * @brief Returns a reference to the camera.
   *
   * @return A reference to the camera.
   */
  Camera &getCamera();

  /**
   * @brief Called on window resize to update framebuffer sizes and pipeline
   * dimensions.
   */
  void resize(int width, int height);

  /**
   * @brief Adds a effect to the current pipeline.
   */
  void addPostProcessingEffect(PostProcessingEffect *effect);

  /**
   * @brief Replaces the entire post–processing pipeline.
   */
  void setPostProcessingPipeline(PostProcessingPipeline *pipeline);

  /**
   * @brief Clears all effects from the current pipeline and creates a new empty
   * pipeline.
   */
  void clearPostProcessingEffects();

  /**
   *  @brief Returns a pointer to the current post–processing pipeline.
   */
  PostProcessingPipeline *getPostProcessingPipeline() const;
};

} // namespace FrameEngine

#endif // RENDERER_HPP
