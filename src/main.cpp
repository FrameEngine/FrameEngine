#include "Engine.hpp"
#include "Logger.hpp"
#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include <algorithm>

Logger &logger = Logger::getInstance();

class Simulation : public Engine {
private:
  Mesh *cubeMesh;
  Object *cube1;
  Object *cube2;

  PointLight *pointLight1;

  float timeElapsed = 0.0f;
  Camera &camera = renderer.getCamera();

public:
  void on_start() override {
    cubeMesh = Mesh::loadFromOBJ("demo_assets/monkey.obj");
    if (!cubeMesh) {
      LOG(ERROR, "Error while loading .obj file");
    }

    // And reuse it :D
    cube1 = new Object(registry, cubeMesh);
    cube1->transform->position = Vector3(0.0f, 0.0f, 0.0f);
    cube1->transform->scale = cube1->transform->scale * .3;
    cube1->rotate(Vector3(0, 1, 0), 180);
    cube1->setColor(Vector3(1, 1, 1));

    cube2 = new Object(registry, cubeMesh);
    cube2->transform->scale = cube1->transform->scale * .5;
    cube2->transform->position = Vector3(1.5f, 2.0f, 1.f);
    cube2->setColor(Vector3(1, 0, 0));

    renderer.submit(cube1);
    renderer.submit(cube2);

    pointLight1 = new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f),
                                 Vector3(1, 1, 1), .7f);
    PointLight *pointLight2 = new PointLight(
        registry, Vector3(5.0f, 5.0f, 0.0f), Vector3(1, 0, 1), 1.5f);
    PointLight *pointLight3 = new PointLight(
        registry, Vector3(-5.0f, 5.0f, 0.0f), Vector3(0, 1, 0), 1.5f);

    renderer.submitLight(pointLight1);
    renderer.submitLight(pointLight2);
    renderer.submitLight(pointLight3);

    camera.transform->position = Vector3(0.f, 0.f, -3.f);
    camera.setProjection(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    cube1->transform->position = Vector3(0, 0, -5.f);
  }

  void fixed_update(float dt) override {
    timeElapsed += dt;

    // cube1->rotate(Vector3(0.5f, 1.0f, 0.0f), dt * 50.0f);
    // cube2->rotate(Vector3(1.0f, 1.0f, 2.0f), dt * 50.0f);
    cube1->transform->position =
        Vector3(sin(timeElapsed) * 5.f, cos(timeElapsed) * 5.f, 0);
    // camera.move(Vector3(-1, 0, 0));

    float radius = .5;
    float ang_speed = 1;
    pointLight1->transform->position =
        (Vector3(radius * cos(ang_speed * timeElapsed), 0,
                 radius * sin(ang_speed * timeElapsed)));

    camera.lookAt(cube1->transform->position);
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
