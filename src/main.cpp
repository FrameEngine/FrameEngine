#include "adapters/X11RendererAdapter.hpp"
#include "core/Engine.hpp"

/* My testing playground */

void Engine::on_start() {}

void Engine::fixed_update() {
  RendererAPI::clear();
  RendererAPI::draw_line(10, 20, 100, 100);
  RendererAPI::present();
}

int main() {
  Engine engine;

  engine.init();
  engine.run();

  return 0;
}
