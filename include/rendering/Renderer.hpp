/**
 * @file Renderer.hpp
 * @brief Manages the rendering pipeline and object submission.
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Mesh.hpp"
#include "Shader.hpp"
#include <memory>
#include <vector>

class Renderer {
private:
  static Shader *shader;
  static std::vector<Mesh *> renderQueue;

public:
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
   * @param mesh The mesh to render.
   * @param shader The shader to use.
   */
  static void submit(Mesh *mesh);

  /**
   * @brief Swaps the front and back buffers.
   *
   * This function should be called at the end of the frame to present
   * the rendered image on the screen.
   */
  static void render();

  /**
   * @brief Shuts down the rendering system and releases resources.
   *
   * This method should be called when the application is closing
   * to properly clean up the rendering context.
   */
  static void shutdown();
};

#endif // RENDERER_HPP
