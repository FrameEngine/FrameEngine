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
  Object *sun;
  Object *earth;
  Object *moon;

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
    sun = new Object(registry, cubeMesh);
    sun->transform->position = Vector3(0.0f, 0.0f, 0.0f);
    sun->transform->scale = sun->transform->scale * 5.f;
    sun->setColor(Vector3(0xff / 256.f, 0xdd / 256.f, 0x40 / 256.f));

    earth = new Object(registry, cubeMesh);
    earth->transform->scale = sun->transform->scale / 109.f * 5.f;
    earth->transform->position = Vector3(2.5f, 0.f, 0.f);
    earth->setColor(Vector3(0, 0, 1));

    moon = new Object(registry, cubeMesh);
    moon->transform->scale = earth->transform->scale * 0.27f;
    moon->transform->position =
        earth->transform->position + Vector3(0.6f, 0.0f, 0.f);
    moon->setColor(Vector3(.8f, .8f, .8f));

    // TODO put in the component. smth like Renderer
    renderer.submit(sun);
    renderer.submit(earth);

    pointLight1 = new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f),
                                 Vector3(1, 1, 0), 10.f);
    PointLight *pointLight2 = new PointLight(
        registry, Vector3(0.0f, 0.0f, 0.0f), Vector3(1, 1, 1), 50.f);

    renderer.submitLight(pointLight1);
    renderer.submitLight(pointLight2);

    camera.transform->position = Vector3(0.f, 50.f, 0.f);
    camera.setProjection(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    camera.lookAt(sun->transform->position);
  }

  void fixed_update(float dt) override { timeElapsed += dt; }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
