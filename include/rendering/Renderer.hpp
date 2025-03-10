/**
 * @file Renderer.hpp
 * @brief Manages the rendering pipeline and object submission.
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Registry.hpp"
#include "Window.hpp"
#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Shader.hpp"
#include <vector>

/**
 * @class Renderer
 * @brief Handles the drawing of scene objects and lights.
 *
 * The Renderer class keeps a list of objects and point lights to render.
 */
class Renderer {
private:
  static Shader *shader; ///< The active shader used for drawing.
  static std::vector<Object *>
      renderQueue; ///< List of objects waiting to be rendered.
  static std::vector<PointLight *>
      lights; ///< List of point lights in the scene.
  Camera camera;
  Window &window;

public:
  /**
   * @brief Constructs a Renderer and sets up the camera.
   *
   * @param window A reference to the Window instance.
   */
  Renderer(Window &window);

  /**
   * @brief Initializes the rendering system.
   *
   * This method must be called before any other rendering function.
   */
  static void initialize();

  /**
   * @brief Clears the screen before rendering a new frame.
   *
   * This should be called at the beginning of each frame to clear
   * the previous frame's contents.
   */
  static void clear();

  /**
   * @brief Submits an object to the rendering queue.
   *
   * @param mesh The mesh to render.
   * @param shader The shader to use.
   */
  static void submit(Object *obj);

  /**
   * @brief Removes all objects from the render queue.
   */
  void clearObjects();

  /**
   * @brief Adds a point light to the list of lights.
   *
   * @param light The point light to submit.
   */
  static void submitLight(PointLight *light);

  /**
   * @brief Renders all submitted objects and lights.
   *
   * This function should be called at the end of the frame to present
   * the rendered image on the screen.
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
   * @brief Sets the active shader.
   *
   * This changes the shader used when drawing objects.
   *
   * @param shader The new shader to use.
   */
  void setShader(Shader *shader);

  /**
   * @brief Returns the currently active shader.
   *
   * @return A pointer to the active shader.
   */
  static Shader *getShader() { return shader; }

  /**
   * @brief Returns a reference to the camera.
   *
   * @return A reference to the camera.
   */
  Camera &getCamera();
};

#endif // RENDERER_HPP
