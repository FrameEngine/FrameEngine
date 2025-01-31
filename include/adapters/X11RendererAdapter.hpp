#ifndef X11_RENDERER_HPP
#define X11_RENDERER_HPP

#include "adapters/IRenderer.hpp"
#include <X11/Xlib.h>

class X11Renderer : public IRenderer {
private:
  Display *display;
  Window window;
  GC gc;

public:
  X11Renderer();
  ~X11Renderer();

  void init() override;
  void clear() override;
  void draw_line(int x1, int y1, int x2, int y2) override;
  void present() override;
};

#endif // X11_RENDERER_HPP
