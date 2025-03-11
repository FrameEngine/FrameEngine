#include "FrameEngine.hpp"
using namespace FrameEngine;

class Simulation : public Engine {
private:
  Mesh *cubeMesh;
  Mesh *sphereMesh;

  Object *cubeObject;
  Object *sphereObject;

  PointLight *pointLight1;
  PointLight *pointLight2;

  float timeElapsed = 0.0f;

public:
  void on_start() override {
    cubeMesh = MeshGenerator::createCube();
    sphereMesh = MeshGenerator::createSphere();

    cubeObject = new Object(cubeMesh);
    cubeObject->setMesh(cubeMesh);
    Material *redMat = new Material();
    redMat->setDiffuseColor(Vector3(1.0f, 0.0f, 0.0f));
    redMat->setSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
    redMat->setSpecularPower(32.0f);
    cubeObject->setMaterial(redMat);
    cubeObject->transform->position = Vector3(-2.0f, 0.0f, 0.0f);

    sphereObject = new Object(sphereMesh);
    sphereObject->setMesh(sphereMesh);
    Material *blueMat = new Material();
    blueMat->setDiffuseColor(Vector3(0.0f, 0.0f, 1.0f));
    blueMat->setSpecularColor(Vector3(1.0f, 1.0f, 1.0f));
    blueMat->setSpecularPower(64.0f);
    sphereObject->setMaterial(blueMat);
    sphereObject->transform->position = Vector3(2.0f, 0.0f, 0.0f);

    renderer.submit(cubeObject);
    renderer.submit(sphereObject);

    pointLight1 = new PointLight(Vector3(-5.0f, 5.0f, 5.0f),
                                 Vector3(1.0f, 1.0f, 1.0f), 3.f);
    pointLight2 = new PointLight(Vector3(5.0f, 5.0f, 5.0f),
                                 Vector3(1.0f, 1.0f, 0.0f), 1.5f);
    renderer.submitLight(pointLight1);
    renderer.submitLight(pointLight2);

    Camera &camera = renderer.getCamera();
    camera.transform->position = Vector3(0.0f, 3.0f, -10.0f);
    camera.setProjection(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    camera.lookAt(Vector3(0, 0, 0));
  }

  void fixed_update(float dt) override {
    timeElapsed += dt;

    float redIntensity = (sin(timeElapsed) + 1.0f) / 2.0f;
    cubeObject->materialComponent->material->setSpecularPower(redIntensity);

    float blueIntensity = (cos(timeElapsed) + 1.0f) / 2.0f;
    sphereObject->materialComponent->material->setSpecularPower(blueIntensity);

    cubeObject->rotate(Vector3(0, 1, 0), 0.5f);
    sphereObject->rotate(Vector3(0, 1, 0), -0.5f);
  }
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
