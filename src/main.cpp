#include "components/MeshComponent.hpp"
#include "core/Engine.hpp"

/* My testing playground */

void Engine::on_start() {
  Entity cube = Engine::regestry.create_entity();
  Engine::regestry.add_component<MeshComponent>(cube, {1});
}

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
