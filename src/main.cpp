#include "Engine.hpp"
#include "Logger.hpp"
#include "MeshGenerator.hpp"
#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include <algorithm>

Logger &logger = Logger::getInstance();

class Simulation : public Engine {
private:
  Mesh *monkeyMesh;
  Object *monkey;
  Object *plane;
  Object *cube;
  Object *sphere;

  PointLight *pointLight1;

  float timeElapsed = 0.0f;
  Camera &camera = renderer.getCamera();

public:
  void on_start() override {
    monkeyMesh = Mesh::loadFromOBJ("demo_assets/monkey.obj");
    if (!monkeyMesh) {
      LOG(ERROR, "Error while loading .obj file");
    }

    plane = new Object(registry, MeshGenerator::createPlane());
    plane->transform->scale *= 10.f;
    cube = new Object(registry, MeshGenerator::createCube());
    cube->setColor(Vector3(1, 0, 0));
    cube->transform->position = Vector3(2.f, 1.f, 0.f);
    sphere = new Object(registry, MeshGenerator::createSphere());
    sphere->setColor(Vector3(1, 0, 1));
    sphere->transform->position = Vector3(-2.f, 1.f, 0.f);
    monkey = new Object(registry, monkeyMesh);
    monkey->setColor(Vector3(0, 1, .5));
    monkey->transform->position = Vector3(0, 1, 0);
    monkey->transform->scale *= .5f;
    monkey->rotate(Vector3(0, 1, 0), 180);

    renderer.submit(plane);
    renderer.submit(cube);
    renderer.submit(sphere);
    renderer.submit(monkey);

    pointLight1 = new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f),
                                 Vector3(1, 1, 0), .7f);
    PointLight *pointLight2 = new PointLight(
        registry, Vector3(0.0f, 3.0f, 0.0f), Vector3(1, 1, 1), 1.f);

    renderer.submitLight(pointLight1);
    renderer.submitLight(pointLight2);

    camera.transform->position = Vector3(0.f, 2.f, -3.f);
    camera.setProjection(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    camera.lookAt(plane->transform->position);
  }

  void fixed_update(float dt) override { timeElapsed += dt; }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
