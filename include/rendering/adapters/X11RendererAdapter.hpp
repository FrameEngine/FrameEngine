/**
 * @file X11RendererAdapter.hpp
 * @brief Implements IRenderer using X11.
 *
 * Function description for override functions can be found in the
 * include/rendering/IRenderer.hpp
 */

#ifndef X11_RENDERER_HPP
#define X11_RENDERER_HPP

#include "rendering/IRenderer.hpp"
#include <X11/Xlib.h>

class X11RendererAdapter : public IRenderer {
public:
  X11RendererAdapter();
  ~X11RendererAdapter();

  void initialize() override;
  void clearScreen() override;
  void drawMesh(const Mesh &mesh, const Matrix4 &transform) override;
  void swapBuffers() override;
  void shutdown() override;

  /**
   * @brief Draws a line between two points in the X11 window.
   * @param x1 The X-coordinate of the first point.
   * @param y1 The Y-coordinate of the first point.
   * @param x2 The X-coordinate of the second point.
   * @param y2 The Y-coordinate of the second point.
   *
   * This function is used internally for drawing edges of a mesh.
   */
  void draw_line(int x1, int y1, int x2, int y2);

  /**
   * @brief Projects a 3D point onto the 2D X11 window.
   * @param point The 3D point in world space.
   * @return The projected 2D coordinates.
   *
   * TODO Research 3D rendering techincs, replace with actual thing.
   */
  Vector3 project(const Vector3 &point);

private:
  /// Pointer to the X11 display (connection to the X server).
  Display *display_;
  /// The X11 window
  Window window_;
  /// Graphics context for drawing operations.
  GC graphicsContext_;
  /// Flag indicating whether the window should close.
  bool shouldClose_;

  /// Resolutution.
  int width, height;
};

#endif // X11_RENDERER_HPP
