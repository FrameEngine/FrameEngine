#ifndef X11_RENDERER_HPP
#define X11_RENDERER_HPP

#include "adapters/IRenderer.hpp"
#include "math/Vector3.hpp"
#include <X11/Xlib.h>

class X11RendererAdapter : public IRenderer {
private:
  Display *display;
  Window window;
  GC gc;
  int screen;
  int width = 800, height = 600;

public:
  X11RendererAdapter();
  ~X11RendererAdapter();

  void init() override;
  void clear() override;
  void draw_line(int x1, int y1, int x2, int y2) override;
  void draw_mesh(const MeshComponent &mesh) override;
  void present() override;

  Vector3 project(const Vector3 &point); // Converts 3D to 2D
};

#endif // X11_RENDERER_HPP
