#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Object.hpp"
#include "math/Vector3.hpp"

/**
 * @class PointLight
 * @brief An ECS entity representing a point light source.
 *
 * PointLight extends Object, automatically inheriting a TransformComponent
 * to manage its spatial position. However, it does not have a mesh associated
 * with it.
 */
class PointLight : public Object {
private:
  Vector3 color;
  float intensity;

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
      : Object(nullptr), color(col), intensity(inten) {
    transform->position = pos;
  }

  /**
   * @brief Retrieves the color of the point light.
   *
   * @return The current color of the light.
   */
  Vector3 getColor() const { return color; }
  /**
   * @brief Sets the color of the point light.
   *
   * @param col The new color for the light.
   */
  void setColor(const Vector3 &col) { color = col; }

  /**
   * @brief Retrieves the intensity of the point light.
   *
   * @return The current intensity of the light.
   */
  float getIntensity() const { return intensity; }

  /**
   * @brief Sets the intensity of the point light.
   *
   * @param inten The new intensity value for the light.
   */
  void setIntensity(float inten) { intensity = inten; }
};

#endif // POINT_LIGHT_HPP
