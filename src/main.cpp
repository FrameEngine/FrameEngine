#include "components/MeshComponent.hpp"
#include "core/Engine.hpp"

/* My testing playground */

Entity cube;

void Engine::on_start() {
  cube = Engine::registry.create_entity();

  // GPT generated mesh, hope it works :D
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

void Engine::fixed_update(float dt) {
  auto &mesh = registry.get_component<MeshComponent>(cube);

  Quaternion rotationX = Quaternion::from_axis_angle({1, 0, 0}, 30.0f * dt);
  Quaternion rotationY = Quaternion::from_axis_angle({0, 1, 0}, 30.0f * dt);
  Quaternion rotationZ = Quaternion::from_axis_angle({0, 0, 1}, 30.0f * dt);

  mesh.rotation = rotationX * rotationY * rotationZ * mesh.rotation;
}

int main() {
  Engine engine;

  engine.init();
  engine.run();

  return 0;
}
