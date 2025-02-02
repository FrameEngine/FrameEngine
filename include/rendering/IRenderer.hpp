/**
 * @file IRenderer.hpp
 * @brief Defines the abstract rendering interface
 *
 * Interface provides a consistent API for different
 * rendering backends. It ensures that all rendering implementations follow the
 * same structure, making it easy to swap rendering engines.
 */

#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "math/Matrix4.hpp"
#include "rendering/Mesh.hpp"

/**
 * @class IRenderer
 * @brief Abstract base class for rendering systems.
 */
class IRenderer {
public:
  virtual ~IRenderer() = default;

  /**
   * @brief Initializes the rendering system.
   *
   * This method must be called before any other rendering function.
   */
  virtual void initialize() = 0;

  /**
   * @brief Clears the screen before rendering a new frame.
   *
   * This should be called at the beginning of each frame to clear
   * the previous frame's contents.
   */
  virtual void clearScreen() = 0;

  /**
   * @brief Draws a mesh using a transformation matrix.
   * @param mesh The mesh to be rendered.
   * @param transform The transformation matrix to apply to the mesh.
   *
   * The transformation matrix determines the mesh's position, rotation,
   * and scale in the world space and therfore on the screen.
   */
  virtual void drawMesh(const Mesh &mesh, const Matrix4 &transform) = 0;

  /**
   * @brief Swaps the front and back buffers.
   *
   * This function should be called at the end of the frame to present
   * the rendered image on the screen.
   */
  virtual void swapBuffers() = 0;

  /**
   * @brief Shuts down the rendering system and releases resources.
   *
   * This method should be called when the application is closing
   * to properly clean up the rendering context.
   */
  virtual void shutdown() = 0;
};

#endif // IRENDERER_HPP
