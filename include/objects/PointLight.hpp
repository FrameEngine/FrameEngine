#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Object.hpp"
#include "components/LightComponent.hpp"
#include "math/Vector3.hpp"

namespace FrameEngine {

/**
 * @class PointLight
 * @brief An ECS entity representing a point light source.
 *
 * PointLight extends Object, automatically inheriting a TransformComponent
 * to manage its spatial position. However, it does not have a mesh associated
 * with it.
 */
class PointLight : public Object {
public:
  /**
   * @brief Constructs a PointLight at a specified position.
   *
   * This constructor creates a PointLight without an associated mesh by passing
   * a null pointer to the base Object constructor. The inherited
   * TransformComponent is used to position the light within the world.
   *
   * @param registry The ECS registry in which this light is registered.
   * @param pos The world-space position of the point light.
   * @param col The color of the light (default is white: (1, 1, 1)).
   * @param inten The intensity of the light (default is 1.0).
   */
  PointLight(const Vector3 &pos, const Vector3 &col = Vector3(1, 1, 1),
             float inten = 1.0f)
      : Object(nullptr) {
    transform->position = pos;
    add_component<LightComponent>(LightComponent(LightType::POINT, col, inten));
  }

  /**
   * @brief Retrieves the color of the point light.
   *
   * @return The current color of the light.
   */
  Vector3 getColor() {
    auto comp = get_component<LightComponent>();
    return comp ? comp->color : Vector3(1, 1, 1);
  }
  /**
   * @brief Sets the color of the point light.
   *
   * @param col The new color for the light.
   */
  void setColor(const Vector3 &col) {
    auto comp = get_component<LightComponent>();
    if (comp)
      comp->color = col;
  }

  /**
   * @brief Retrieves the intensity of the point light.
   *
   * @return The current intensity of the light.
   */
  float getIntensity() {
    auto comp = get_component<LightComponent>();
    return comp ? comp->intensity : 1.0f;
  }

  /**
   * @brief Sets the intensity of the point light.
   *
   * @param inten The new intensity value for the light.
   */
  void setIntensity(float inten) {
    auto comp = get_component<LightComponent>();
    if (comp)
      comp->intensity = inten;
  }
};

} // namespace FrameEngine

#endif // POINT_LIGHT_HPP
