#include "Engine.hpp"
#include "Object.hpp"
#include "math/Matrix4.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"

class Simulation : public Engine {
private:
  Mesh *cubeMesh;
  Object *cube1;
  Object *cube2;

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
    // Create a single cube mesh
    cubeMesh =
        new Mesh(cubeVertices, cubeIndices, 8, 36, Vector3(1.0f, 0.5f, 0.2f));

    // And reuse it :D
    cube1 = new Object(registry, cubeMesh);
    cube1->transform->position = Vector3(0.0f, 0.0f, 0.0f);

    renderer.submit(cube1);

    Camera camera = renderer.getCamera();
    camera.setPosition(Vector3(0.0f, 2.0f, -4.0f));
    camera.lookAt(Vector3(0.0f, 0.0f, 0.0f));
  }

  void fixed_update(float dt) override {
    cube1->rotate(Vector3(0.5f, 1.0f, 0.0f), dt * 50.0f);
    cube2->rotate(Vector3(1.0f, 0.0f, 0.5f), dt * 30.0f);
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
