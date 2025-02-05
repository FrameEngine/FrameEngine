#include "components/PhysicsComponent.hpp"
#include "core/Engine.hpp"
#include "core/Object.hpp"
#include "physics/PhysicsSystem.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/adapters/X11RendererAdapter.hpp"

class Simulation : public Engine {
private:
  Object *cube;
  Object *spheres[3];

  float orbitRadius = 3.0f;
  float cubeMass = 5.0f;
  float gravityConstant = 5.0f;

public:
  void on_start() override {
    // Create cube
    cube = new Object(registry, Mesh::createCube());
    cube->transform->position = Vector3(1.0f, -2.0f, -7.0f);
    cube->add_component<PhysicsComponent>(Vector3(), Vector3(), cubeMass);

    // Create spheres
    for (int i = 0; i < 3; i++) {
      spheres[i] = new Object(registry, Mesh::createSphere());
      spheres[i]->add_component<PhysicsComponent>(Vector3(), Vector3(), 1.0f);
    }

    // Place spheres around cube with initial orbital velocity
    spheres[0]->transform->position =
        cube->transform->position + Vector3(orbitRadius, 0.0f, 0.0f);
    spheres[1]->transform->position =
        cube->transform->position + Vector3(-orbitRadius, 0.0f, 0.0f);
    spheres[2]->transform->position =
        cube->transform->position + Vector3(orbitRadius / 2, orbitRadius, 0.0f);

    // Stable orbits velocity (GM/r)
    float initialSpeed = sqrt(gravityConstant * cubeMass / orbitRadius);
    spheres[0]->get_component<PhysicsComponent>()->velocity =
        Vector3(0.0f, initialSpeed, 0.0f);
    spheres[1]->get_component<PhysicsComponent>()->velocity =
        Vector3(0.0f, -initialSpeed, 0.0f);
    spheres[2]->get_component<PhysicsComponent>()->velocity =
        Vector3(-initialSpeed, 0.0f, 0.0f);
  }

  void fixed_update(float dt) override {
    // Rotate cube over time
    Quaternion rotationStep =
        Quaternion::from_axis_angle(Vector3(.5, 1, 1), 45.0f * dt);
    cube->transform->rotation = rotationStep * cube->transform->rotation;

    for (int i = 0; i < 3; i++) {
      auto &spherePhysics = *spheres[i]->get_component<PhysicsComponent>();

      // Gravitational force
      Vector3 direction =
          cube->transform->position - spheres[i]->transform->position;
      float distance = direction.magnitude();
      if (distance > 0.1f) { // Avoid extreme forces at very small distances
        Vector3 force = direction.normalized() *
                        (gravityConstant * cubeMass / (distance * distance));

        spherePhysics.acceleration = force;
      }

      // Rotate spheres
      Quaternion selfRotationStep =
          Quaternion::from_axis_angle(Vector3((random() % 90 + 10) / 100.f, 1,
                                              (random() % 90 + 10) / 100.f),
                                      90.0f * dt);
      spheres[i]->transform->rotation =
          selfRotationStep * spheres[i]->transform->rotation;
    }

    // Apply physics updates
    PhysicsSystem::update(registry, dt);
  }
};

int main() {
  X11RendererAdapter X11Renderer;
  Renderer::set_renderer(&X11Renderer);
  Renderer::init();

  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
