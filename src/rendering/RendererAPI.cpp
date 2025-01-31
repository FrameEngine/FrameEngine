#include "rendering/RendererAPI.hpp"
#include "adapters/X11RendererAdapter.hpp"

// For now I'll use X11, but in future it is possible to add openGL support
// (probably :D)
static X11RendererAdapter defaultRenderer;

IRenderer *RendererAPI::renderer = &defaultRenderer;

void RendererAPI::set_renderer(IRenderer *newRenderer) {
  if (newRenderer) {
    renderer = newRenderer;
  }
}

void RendererAPI::init() { renderer->init(); }

void RendererAPI::clear() { renderer->clear(); }

void RendererAPI::draw_line(int x1, int y1, int x2, int y2) {
  renderer->draw_line(x1, y1, x2, y2);
}

void RendererAPI::draw_mesh(const MeshComponent &mesh) {
  renderer->draw_mesh(mesh);
}

void RendererAPI::present() { renderer->present(); }
