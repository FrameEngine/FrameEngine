#include "rendering/Renderer.hpp"
#include "objects/Object.hpp"
#include <glad/glad.h>

/**
 * @brief Static member definitions.
 */
Shader *Renderer::shader = nullptr; ///< Pointer to the active shader.
std::vector<Object *>
    Renderer::renderQueue; ///< List of objects to be rendered.
std::vector<PointLight *>
    Renderer::lights; ///< List of point lights in the scene.

/**
 * @brief Constructs a Renderer.
 *
 * @param registry The ECS registry used for initializing the camera.
 */
// TODO SO far it's hardcoded, replace with actual width and height
Renderer::Renderer(Registry &registry)
    : camera(Camera(registry, 1920.0f / 1080.0f)) {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  // The default shader.
  shader =
      new Shader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
}

/**
 * @brief Returns a reference to the camera.
 *
 * @return A reference to the camera.
 */
Camera &Renderer::getCamera() { return camera; }

/**
 * @brief Sets the active shader.
 *
 * This changes the shader used when drawing objects.
 *
 * @param shader The new shader to use.
 */
void Renderer::setShader(Shader *newShader) {
  if (shader) {
    delete shader;
  }
  shader = newShader;
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

/**
 * @brief Submits an object to the rendering queue.
 *
 * @param mesh The mesh to render.
 * @param shader The shader to use.
 */
void Renderer::submit(Object *obj) { renderQueue.push_back(obj); }

/**
 * @brief Adds a point light to the list of lights.
 *
 * @param light The point light to submit.
 */
void Renderer::submitLight(PointLight *light) { lights.push_back(light); }

/**
 * @brief Renders all submitted objects and lights.
 *
 * This function should be called at the end of the frame to present
 * the rendered image on the screen.
 */
void Renderer::render() {
  shader->bind();

  // Pass data to shaders.
  shader->setUniformMat4("view", camera.getViewMatrix());
  shader->setUniformMat4("projection", camera.getProjectionMatrix());
  shader->setUniformInt("numLights", lights.size());

  // For each light, send its position, color, and intensity to the shader.
  for (size_t i = 0; i < lights.size(); ++i) {
    std::string lightIndex = "lights[" + std::to_string(i) + "]";
    shader->setUniformVec3(lightIndex + ".position",
                           lights[i]->transform->position);
    shader->setUniformVec3(lightIndex + ".color", lights[i]->getColor());
    shader->setUniformFloat(lightIndex + ".intensity",
                            lights[i]->getIntensity());
  }

  // Render each object in the render queue.
  for (auto *obj : renderQueue) {
    obj->render(*this);
  }

  shader->unbind();
}

/**
 * @brief Removes all objects from the render queue.
 */
void Renderer::clearObjects() { renderQueue.clear(); }

/**
 * @brief Shuts down the rendering system and releases resources.
 *
 * This method should be called when the application is closing
 * to properly clean up the rendering context.
 */
void Renderer::shutdown() {
  delete shader;
  renderQueue.clear();
  lights.clear();
}
