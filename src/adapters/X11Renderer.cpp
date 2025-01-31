#include "adapters/X11RendererAdapter.hpp"
#include <iostream>
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

void X11RendererAdapter::draw_mesh(int meshID) {
  // Placeholder for future 3D rendering
  // For now, just draw a simple square
  XFillRectangle(display, window, gc, 100, 100, 50, 50);
}

void X11RendererAdapter::present() { XFlush(display); }
