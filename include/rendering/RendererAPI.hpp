#ifndef RENDERER_API_HPP
#define RENDERER_API_HPP

#include "adapters/IRenderer.hpp"

class RendererAPI {
private:
  static IRenderer *renderer;

public:
  static void set_renderer(IRenderer *newRenderer);
  static void init();
  static void clear();
  static void draw_line(int x1, int y1, int x2, int y2);
  static void draw_mesh(const MeshComponent &mesh);
  static void present();
};

#endif // RENDERER_API_HPP
