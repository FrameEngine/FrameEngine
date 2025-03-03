#include "Engine.hpp"
#include "Logger.hpp"
#include "objects/Object.hpp"
#include "objects/PointLight.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Mesh.hpp"
#include <cmath>

// A simple three-body gravitational simulation.
class Simulation : public Engine {
private:
  Mesh *mesh;
  Object *body1;
  Object *body2;
  Object *body3;
  PointLight *cameraLight;

  // We'll simulate in the XZ plane (Y remains 0 for bodies)
  // and use a top-down camera along positive Y.
  // Use a simple Euler integration for updating positions.
  float mass1, mass2, mass3;
  Vector3 velocity1, velocity2, velocity3;
  const float G = 1.0f; // Gravitational constant (arbitrary units)
  const float scale =
      20.0f; // Scale factor to enlarge initial positions/velocities
  const float cameraHeight =
      50.0f; // Camera elevation above the system's center

  Camera &camera = renderer.getCamera();

public:
  void on_start() override {
    // Load a mesh (e.g. a monkey head) for visualization
    mesh = Mesh::loadFromOBJ("demo_assets/monkey.obj");
    if (!mesh) {
      LOG(ERROR, "Error loading mesh");
    }

    // Create three bodies
    body1 = new Object(registry, mesh);
    body2 = new Object(registry, mesh);
    body3 = new Object(registry, mesh);

    // Set visual scales (adjust as needed)
    body1->transform->scale = Vector3(1.0f, 1.0f, 1.0f);
    body2->transform->scale = Vector3(1.0f, 1.0f, 1.0f);
    body3->transform->scale = Vector3(1.0f, 1.0f, 1.0f);

    // --- Figure-eight initial conditions for three equal masses ---
    // Positions (using X and Z; Y = 0)
    // (These are the normalized values multiplied by our scale factor.)
    body1->transform->position =
        Vector3(0.97000436f * scale, 0.0f, -0.24308753f * scale);
    body2->transform->position =
        Vector3(-0.97000436f * scale, 0.0f, 0.24308753f * scale);
    body3->transform->position = Vector3(0.0f, 0.0f, 0.0f);

    // Velocities (again, in the XZ plane)
    velocity1 = Vector3(0.4662036850f * scale, 0.0f, 0.43236573f * scale);
    velocity2 = Vector3(0.4662036850f * scale, 0.0f, 0.43236573f * scale);
    velocity3 = Vector3(-0.93240737f * scale, 0.0f, -0.86473146f * scale);

    // All bodies have equal masses.
    mass1 = mass2 = mass3 = 1.0f;

    // Submit bodies to the renderer.
    renderer.submit(body1);
    renderer.submit(body2);
    renderer.submit(body3);

    // Set up the camera for a top-down view.
    // Compute the initial center of mass.
    Vector3 centerOfMass =
        (body1->transform->position + body2->transform->position +
         body3->transform->position) /
        3.0f;
    camera.transform->position =
        centerOfMass + Vector3(0.0f, cameraHeight, 0.0f);
    camera.lookAt(centerOfMass);

    // Create a point light that is placed at the camera's position.
    cameraLight = new PointLight(registry, camera.transform->position,
                                 Vector3(1.0f, 1.0f, 1.0f), 100.0f);
    renderer.submitLight(cameraLight);
  }

  void fixed_update(float dt) override {
    // Retrieve current positions
    Vector3 pos1 = body1->transform->position;
    Vector3 pos2 = body2->transform->position;
    Vector3 pos3 = body3->transform->position;

    // Calculate gravitational accelerations.
    Vector3 a1(0, 0, 0), a2(0, 0, 0), a3(0, 0, 0);

    // Acceleration on body1 due to body2 and body3
    {
      Vector3 r12 = pos2 - pos1;
      float dist12 = r12.magnitude();
      if (dist12 > 0) {
        a1 += G * mass2 * r12 / (dist12 * dist12 * dist12);
      }
      Vector3 r13 = pos3 - pos1;
      float dist13 = r13.magnitude();
      if (dist13 > 0) {
        a1 += G * mass3 * r13 / (dist13 * dist13 * dist13);
      }
    }

    // Acceleration on body2 due to body1 and body3
    {
      Vector3 r21 = pos1 - pos2;
      float dist21 = r21.magnitude();
      if (dist21 > 0) {
        a2 += G * mass1 * r21 / (dist21 * dist21 * dist21);
      }
      Vector3 r23 = pos3 - pos2;
      float dist23 = r23.magnitude();
      if (dist23 > 0) {
        a2 += G * mass3 * r23 / (dist23 * dist23 * dist23);
      }
    }

    // Acceleration on body3 due to body1 and body2
    {
      Vector3 r31 = pos1 - pos3;
      float dist31 = r31.magnitude();
      if (dist31 > 0) {
        a3 += G * mass1 * r31 / (dist31 * dist31 * dist31);
      }
      Vector3 r32 = pos2 - pos3;
      float dist32 = r32.magnitude();
      if (dist32 > 0) {
        a3 += G * mass2 * r32 / (dist32 * dist32 * dist32);
      }
    }

    // Update velocities using Euler integration.
    velocity1 += a1 * dt;
    velocity2 += a2 * dt;
    velocity3 += a3 * dt;

    // Update positions using the new velocities.
    pos1 += velocity1 * dt;
    pos2 += velocity2 * dt;
    pos3 += velocity3 * dt;

    // Assign the updated positions back to the objects.
    body1->transform->position = pos1;
    body2->transform->position = pos2;
    body3->transform->position = pos3;

    // Compute the system's center of mass.
    Vector3 centerOfMass = (pos1 + pos2 + pos3) / 3.0f;

    // Update the camera to stay top-down over the center.
    camera.transform->position =
        centerOfMass + Vector3(0.0f, cameraHeight, 0.0f);
    camera.lookAt(centerOfMass);

    // Move the light along with the camera.
    cameraLight->transform->position = camera.transform->position;
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
