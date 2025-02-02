/**
 * @file Renderer.hpp
 * @brief Provides a global interface for rendering operations.
 *
 * The Renderer class is a static wrapper that delegates rendering tasks
 * to a dynamically assigned backend (IRenderer). This allows flexibility in
 * choosing the rendering implementation without modifying domain logic
 *
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "IRenderer.hpp"

/**
 * @class Renderer
 * @brief A static interface for rendering operations.
 *
 * The Renderer class provides a global API for rendering without exposing
 * details of the rendering backend.
 */
class Renderer {
private:
  static IRenderer *renderer;

public:
  /**
   * @brief Sets the active rendering backend.
   * @param newRenderer Pointer to an IRenderer implementation.
   *
   * This function **dynamically assigns a renderer** at runtime, allowing
   * easy switching between different rendering backends.
   *
   * Example:
   *
   * ```
   * X11RendererAdapter x11Renderer;
   * Renderer::set_renderer(&x11Renderer);
   * ```
   *
   */
  static void set_renderer(IRenderer *newRenderer);

  static void init();
  static void clear();
  static void draw_mesh(Mesh &mesh, const Matrix4 &transformMatrix);
  static void present();
  static void shutdown();
};

#endif // RENDERER_HPP
