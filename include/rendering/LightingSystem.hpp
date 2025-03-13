#ifndef LIGHTING_SYSTEM_HPP
#define LIGHTING_SYSTEM_HPP

#include "Logger.hpp"
#include "Registry.hpp"
#include "components/LightComponent.hpp"
#include "components/TransformComponent.hpp"
#include "math/Vector3.hpp"
#include <cstdio>
#include <glad/glad.h>

namespace FrameEngine {

constexpr int MAX_LIGHTS = 10;

struct alignas(16) LightData {
  float position[4]; // x, y, z, padding
  float color[4];    // r, g, b, intensity
};

struct alignas(16) LightingUBOData {
  int numLights;
  int padding[3];
  LightData lights[MAX_LIGHTS];
};

class LightingSystem {
private:
  static GLuint ubo;              ///< UBO handle.
  static LightingUBOData uboData; ///< CPU-side UBO data.
public:
  /**
   * @brief Initializes the lighting system by creating the UBO.
   */
  static void initialize() {
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightingUBOData), nullptr,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
  }

  /**
   * @brief Updates the lighting UBO with data from all entities that have a
   * LightComponent.
   *
   * This function queries the global Registry for entities with both
   * LightComponent and TransformComponent, fills the uboData structure
   * accordingly, and uploads it to the GPU.
   */
  static void update() {
    Registry &registry = Registry::getInstance();
    auto lightEntities = registry.get_entities_with_component<LightComponent>();

    int count = 0;
    for (EntityID entity : lightEntities) {
      if (count >= MAX_LIGHTS)
        break;
      if (!registry.has_component<TransformComponent>(entity))
        continue;

      auto &lightComp = registry.get_component<LightComponent>(entity);
      auto &transform = registry.get_component<TransformComponent>(entity);

      uboData.lights[count].position[0] = transform.position.x;
      uboData.lights[count].position[1] = transform.position.y;
      uboData.lights[count].position[2] = transform.position.z;
      uboData.lights[count].position[3] = 0.0f; // padding

      uboData.lights[count].color[0] = lightComp.color.x;
      uboData.lights[count].color[1] = lightComp.color.y;
      uboData.lights[count].color[2] = lightComp.color.z;
      uboData.lights[count].color[3] = lightComp.intensity;

      count++;
    }
    uboData.numLights = count;

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightingUBOData), &uboData);
  }

  /**
   * @brief Releases the UBO resources.
   */
  static void shutdown() {
    if (ubo != 0) {
      glDeleteBuffers(1, &ubo);
      ubo = 0;
    }
  }
};

} // namespace FrameEngine

#endif // LIGHTING_SYSTEM_HPP
