#include "components/MeshComponent.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"
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

  TransformComponent transform;
  transform.position = Vector3(2, 0, 0);
  Engine::registry.add_component<TransformComponent>(cube, transform);

  PhysicsComponent physics(Vector3(0.0f, 0.0f, 1.0f),
                           Vector3(0.0f, -1.0f, 0.0f));
  Engine::registry.add_component<PhysicsComponent>(cube, physics);
}

void Engine::fixed_update(float dt) {
  auto &transform = Engine::registry.get_component<TransformComponent>(cube);
  auto &physics = Engine::registry.get_component<PhysicsComponent>(cube);
  auto &mesh = Engine::registry.get_component<MeshComponent>(cube);

  // Apply rotation
  Quaternion rotationX = Quaternion::from_axis_angle({1, 0, 0}, 30.0f * dt);
  Quaternion rotationY = Quaternion::from_axis_angle({0, 1, 0}, 30.0f * dt);
  Quaternion rotationZ = Quaternion::from_axis_angle({0, 0, 1}, 30.0f * dt);

  transform.rotation = rotationX * rotationY * rotationZ * transform.rotation;

  // Apply physics
  physics.velocity = physics.velocity + physics.acceleration * dt;
  transform.position = transform.position + physics.velocity * dt;

  // Apply Transform
  for (size_t i = 0; i < mesh.vertices.size(); i++) {
    Vector3 localVertex = mesh.vertices[i];
    Vector3 rotatedVertex = transform.rotation.rotate(localVertex);
    mesh.transformedVertices[i] = rotatedVertex + transform.position;
  }
}

int main() {
  Engine engine;

  engine.init();
  engine.run();

  return 0;
}
