/**
 * @file X11RendererAdapter.cpp
 * @brief Implements the X11-based rendering backend for ECS simulations.
 *
 * This class is designed to be easily replaceable with more advanced
 * rendering backends like OpenGL or Vulkan.
 */

#include "rendering/adapters/X11RendererAdapter.hpp"
#include <iostream>

/**
 * @brief Constructs the X11 renderer and initializes internal variables.
 *
 * This does **not** create a window yet; initialize() must be called to set up
 * X11.
 */
X11RendererAdapter::X11RendererAdapter()
    : display_(nullptr), window_(0), graphicsContext_(0), shouldClose_(false),
      width(800), height(600) {}

X11RendererAdapter::~X11RendererAdapter() { shutdown(); }

/**
 * @brief Initializes the X11 rendering system.
 *
 * This function:
 * - Connects to the X11 display server.
 * - Creates a new X11 window.
 * - Sets up a graphics context (GC) for drawing.
 */
void X11RendererAdapter::initialize() {
  display_ = XOpenDisplay(nullptr);
  if (!display_) {
    std::cerr << "Error: Cannot open X display\n";
    return;
  }

  int screen = DefaultScreen(display_);
  window_ = XCreateSimpleWindow(display_, RootWindow(display_, screen), 10, 10,
                                width, height, 1, BlackPixel(display_, screen),
                                WhitePixel(display_, screen));

  XMapWindow(display_, window_);
  XFlush(display_);
  graphicsContext_ = XCreateGC(display_, window_, 0, nullptr);
}

/**
 * @brief Clears the screen by filling it with a background color.
 */
void X11RendererAdapter::clearScreen() { XClearWindow(display_, window_); }

/**
 * @brief Draws a line between two points in the X11 window.
 * @param x1 The X-coordinate of the first point.
 * @param y1 The Y-coordinate of the first point.
 * @param x2 The X-coordinate of the second point.
 * @param y2 The Y-coordinate of the second point.
 *
 * This function is used internally for drawing edges of a mesh.
 */
void X11RendererAdapter::draw_line(int x1, int y1, int x2, int y2) {
  XDrawLine(display_, window_, graphicsContext_, x1, y1, x2, y2);
}

/**
 * @brief Projects a 3D point onto the 2D X11 window.
 * @param point The 3D point in world space.
 * @return The projected 2D coordinates.
 *
 * This function applies a basic perspective projection:
 *
 * ```
 * screenX = (width / 2) + (point.x * scaleFactor / point.z)
 * screenY = (height / 2) - (point.y * scaleFactor / point.z)
 * ```
 *
 * This ensures that objects farther from the camera appear smaller.
 */
Vector3 X11RendererAdapter::project(const Vector3 &point) {
  float scale = point.z * 100.0f;         // Scale factor for projection
  return {(width / 2) + point.x * scale,  // Centered X
          (height / 2) - point.y * scale, // Inverted Y
          point.z};
}

/**
 * @brief Draws a mesh using a transformation matrix.
 * @param mesh The mesh to render.
 * @param transform The transformation matrix to apply.
 *
 * This function:
 * - Applies the transformation matrix to all vertices.
 * - Projects the transformed points into 2D screen space.
 * - Draws the edges of the mesh as connected lines.
 */
void X11RendererAdapter::drawMesh(const Mesh &mesh, const Matrix4 &transform) {
  for (const auto &edge : mesh.edges) {
    Vector3 v1_transformed =
        transform.transform(mesh.vertices[edge.first].position);
    Vector3 v2_transformed =
        transform.transform(mesh.vertices[edge.second].position);

    if (v1_transformed.z != 0) {
      v1_transformed.x /= v1_transformed.z;
      v1_transformed.y /= v1_transformed.z;
    }
    if (v2_transformed.z != 0) {
      v2_transformed.x /= v2_transformed.z;
      v2_transformed.y /= v2_transformed.z;
    }

    Vector3 v1_screen = project(v1_transformed);
    Vector3 v2_screen = project(v2_transformed);

    draw_line(static_cast<int>(v1_screen.x), static_cast<int>(v1_screen.y),
              static_cast<int>(v2_screen.x), static_cast<int>(v2_screen.y));
  }
}

/**
 * @brief Swaps buffers to present the rendered frame.
 *
 * X11 does not have built-in double buffering, so this function
 * is a placeholder.
 */
void X11RendererAdapter::swapBuffers() { XFlush(display_); }

/**
 * @brief Shuts down the renderer and releases X11 resources.
 */
void X11RendererAdapter::shutdown() {
  if (graphicsContext_)
    XFreeGC(display_, graphicsContext_);
  if (window_)
    XDestroyWindow(display_, window_);
  if (display_)
    XCloseDisplay(display_);
  display_ = nullptr;
}
