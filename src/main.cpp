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
    // ChatGPT generated cube
    float cubeVertices[] = {
        // X      Y      Z
        -0.5f, -0.5f, -0.5f, // 0 - Bottom-left-back
        0.5f,  -0.5f, -0.5f, // 1 - Bottom-right-back
        0.5f,  0.5f,  -0.5f, // 2 - Top-right-back
        -0.5f, 0.5f,  -0.5f, // 3 - Top-left-back

        -0.5f, -0.5f, 0.5f, // 4 - Bottom-left-front
        0.5f,  -0.5f, 0.5f, // 5 - Bottom-right-front
        0.5f,  0.5f,  0.5f, // 6 - Top-right-front
        -0.5f, 0.5f,  0.5f  // 7 - Top-left-front
    };

    unsigned int cubeIndices[] = {
        0, 1, 2, 2, 3, 0, // Back face
        4, 5, 6, 6, 7, 4, // Front face
        0, 1, 5, 5, 4, 0, // Bottom face
        2, 3, 7, 7, 6, 2, // Top face
        0, 3, 7, 7, 4, 0, // Left face
        1, 2, 6, 6, 5, 1  // Right face
    };

    float cubeNormals[] = {};

    cubeMesh = new Mesh(cubeVertices, cubeNormals, cubeIndices, 8, 36);

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

    pointLight = new PointLight(registry, Vector3(5.0f, 5.0f, 0.0f));
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
