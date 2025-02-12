#include "Engine.hpp"
#include "objects/Object.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"

class Simulation : public Engine {
private:
  Mesh *cubeMesh;
  Object *cube1;
  Object *cube2;

  PointLight *pointLight;

  float timeElapsed = 0.0f;
  Camera &camera = renderer.getCamera();

public:
  void on_start() override {
    cubeMesh = Mesh::loadFromOBJ("demo_assets/monkey.obj");
    if (!cubeMesh) {
      std::cout << "Error while loading .obj file" << std::endl;
    }

    // And reuse it :D
    cube1 = new Object(registry, cubeMesh);
    cube1->transform->position = Vector3(0.0f, 0.0f, 0.0f);
    cube1->transform->scale = cube1->transform->scale * .3;

    cube2 = new Object(registry, cubeMesh);
    cube2->transform->scale = cube1->transform->scale * .5;
    cube2->transform->position = Vector3(1.5f, 2.0f, 3);
    cube2->setColor(Vector3(1, 0, 0));

    renderer.submit(cube1);
    renderer.submit(cube2);

    pointLight = new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f),
                                Vector3(1, 1, 1), 3.f);
    renderer.submitLight(pointLight);

    camera.setPosition(Vector3(0, 0, -2.0f));
    camera.lookAt(Vector3(0.0f, 0.0f, 0.0f));
  }

  void fixed_update(float dt) override {
    timeElapsed += dt;

    cube1->rotate(Vector3(0.5f, 1.0f, 0.0f), dt * 50.0f);
    cube2->rotate(Vector3(1.0f, 1.0f, 2.0f), dt * 50.0f);
    cube2->move(Vector3(sin(timeElapsed) / 20.0f, 0, 0));
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
