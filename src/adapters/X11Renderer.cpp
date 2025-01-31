#include "adapters/X11Renderer.hpp"
#include <stdexcept>

X11Renderer::X11Renderer() : display(nullptr), window(0), gc(0) {}

X11Renderer::~X11Renderer() {
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

/* TODO Litteraly everything is const right now, rewrite and make it accept at
 * least window size parameters. Background color etc*/
void X11Renderer::init() {
  display = XOpenDisplay(NULL);
  if (!display)
    throw std::runtime_error("Failed to open X11 display");

  int screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10,
                               800, 600, 1, BlackPixel(display, screen),
                               WhitePixel(display, screen));
  gc = XCreateGC(display, window, 0, nullptr);
  XMapWindow(display, window);
}

void X11Renderer::clear() { XClearWindow(display, window); }

void X11Renderer::draw_line(int x1, int y1, int x2, int y2) {
  XDrawLine(display, window, gc, x1, y1, x2, y2);
}

void X11Renderer::present() { XFlush(display); }
