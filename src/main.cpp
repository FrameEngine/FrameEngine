#include "components/MeshComponent.hpp"
#include "core/Engine.hpp"

/* My testing playground */

void Engine::on_start() {
  Entity cube = Engine::registry.create_entity();

  // GPT generated, hope it works :D
  MeshComponent cubeMesh = {// Vertices
                            {
                                {-1, -1, -1},
                                {1, -1, -1},
                                {1, 1, -1},
                                {-1, 1, -1}, // Back face
                                {-1, -1, 1},
                                {1, -1, 1},
                                {1, 1, 1},
                                {-1, 1, 1} // Front face
                            },
                            {
                                {0, 1},
                                {1, 2},
                                {2, 3},
                                {3, 0}, // Back face
                                {4, 5},
                                {5, 6},
                                {6, 7},
                                {7, 4}, // Front face
                                {0, 4},
                                {1, 5},
                                {2, 6},
                                {3, 7} // Connecting edges
                            }};
  Engine::registry.add_component<MeshComponent>(cube, {cubeMesh});
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
