#include "Engine.hpp"
#include "math/Vector3.hpp"
#include "rendering/Mesh.hpp"

class Simulation : public Engine {
private:
  Mesh *triangleMesh;

public:
  void on_start() override {
    float triangleVertices[] = {
        0.0f,  0.5f,  0.0f, // Top
        -0.5f, -0.5f, 0.0f, // Bottom Left
        0.5f,  -0.5f, 0.0f  // Bottom Right
    };

    Vector3 color(1.0f, 0.5f, 0.2f);
    triangleMesh = new Mesh(triangleVertices, 3, color);

    Renderer::submit(triangleMesh);
  }

  void fixed_update(float dt) override {}

  ~Simulation() { delete triangleMesh; }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
