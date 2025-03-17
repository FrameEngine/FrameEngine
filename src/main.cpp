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

  Texture2D *texture;

  float timeElapsed = 0.0f;

public:
  void on_start() override {
    Shader *lightingShader =
        new FrameEngine::Shader("shaders/basic.vs", "shaders/basic.fs");

    texture = new Texture2D();
    if (!texture->loadFromFile("assets/wood_texture.jpg")) {
      LOG(ERROR, "Failed to load texture!");
    }

    cubeMesh = MeshGenerator::createCube();
    sphereMesh = MeshGenerator::createSphere();

    cubeObject = new Object(cubeMesh);
    cubeObject->setMesh(cubeMesh);

    BasicMaterial *redMat = new FrameEngine::BasicMaterial(
        lightingShader, Vector3(1, 0, 0), Vector3(1, 1, 1), 64.0f);

    redMat->setEmissiveColor(Vector3(0, 1, 0));
    redMat->setEmissiveEnabled(true);
    cubeObject->setMaterial(redMat);
    cubeObject->transform->position = Vector3(-2.0f, 0.0f, 0.0f);

    sphereObject = new Object(sphereMesh);
    sphereObject->setMesh(sphereMesh);

    BasicMaterial *blueMat = new FrameEngine::BasicMaterial(
        lightingShader, Vector3(1, 1, 1), Vector3(1, 1, 1), 32.0f);
    blueMat->setTexture(texture);

    sphereObject->setMaterial(blueMat);
    sphereObject->transform->position = Vector3(2.0f, 0.0f, 0.0f);
    sphereObject->transform->scale *= 3.f;

    pointLight1 = new PointLight(Vector3(-5.0f, 5.0f, 5.0f),
                                 Vector3(1.0f, 1.0f, 1.0f), 3.f);
    // pointLight2 = new PointLight(Vector3(5.0f, 5.0f, 5.0f),
    //                              Vector3(1.0f, 1.0f, 0.0f), 1.5f);

    Camera &camera = renderer.getCamera();
    camera.transform->position = Vector3(0.0f, 3.0f, -10.0f);
    camera.setProjection(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    camera.lookAt(Vector3(0, 0, 0));
  }

  void fixed_update(float dt) override {
    timeElapsed += dt;

    pointLight1->transform->position =
        Vector3(5.0f * sin(timeElapsed), 3, 5 * cos(timeElapsed));

    // float redIntensity = (sin(timeElapsed) + 1.0f) / 2.0f;
    // cubeObject->materialComponent->material->setSpecularPower(redIntensity);
    //
    // float blueIntensity = (cos(timeElapsed) + 1.0f) / 2.0f;
    // sphereObject->materialComponent->material->setSpecularPower(blueIntensity);

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
