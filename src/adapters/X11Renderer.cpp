#include "adapters/X11RendererAdapter.hpp"
#include "math/Matrix4.hpp"
#include <stdexcept>
#include <unistd.h>

X11RendererAdapter::X11RendererAdapter() : display(nullptr), window(0), gc(0) {}

X11RendererAdapter::~X11RendererAdapter() {
  if (gc) {
    XFreeGC(display, gc);
  }
  if (window) {
    XDestroyWindow(display, window);
  }
  if (display) {
    XCloseDisplay(display);
  }
}

void X11RendererAdapter::init() {
  display = XOpenDisplay(nullptr);
  if (!display) {
    throw std::runtime_error("Failed to open X11 display");
  }

  screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10,
                               800, 600, 1, BlackPixel(display, screen),
                               WhitePixel(display, screen));

  gc = XCreateGC(display, window, 0, nullptr);
  XSetForeground(display, gc, BlackPixel(display, screen));

  // Make the window visible
  XMapWindow(display, window);
}

void X11RendererAdapter::clear() { XClearWindow(display, window); }

void X11RendererAdapter::draw_line(int x1, int y1, int x2, int y2) {
  XDrawLine(display, window, gc, x1, y1, x2, y2);
}

// Simple orthographic projection
Vector3 X11RendererAdapter::project(const Vector3 &point) {
  float scale = 100.0f;                   // Scale factor for projection
  return {(width / 2) + point.x * scale,  // Centered X
          (height / 2) - point.y * scale, // Inverted Y
          point.z};
}

void X11RendererAdapter::draw_mesh(const MeshComponent &mesh) {
  Matrix4 rotationMatrix = Matrix4::from_quaternion(mesh.rotation);

  std::vector<Vector3> transformedVertices;
  for (const auto &vertex : mesh.vertices) {
    transformedVertices.push_back(rotationMatrix.transform(vertex));
  }

  for (const auto &edge : mesh.edges) {
    Vector3 v1 = project(transformedVertices[edge.first]);
    Vector3 v2 = project(transformedVertices[edge.second]);

    draw_line(static_cast<int>(v1.x), static_cast<int>(v1.y),
              static_cast<int>(v2.x), static_cast<int>(v2.y));
  }
}

void X11RendererAdapter::present() { XFlush(display); }
