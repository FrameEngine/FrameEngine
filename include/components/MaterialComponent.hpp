#ifndef MATERIAL_COMPONENT_HPP
#define MATERIAL_COMPONENT_HPP

#include "math/Vector3.hpp"
#include "rendering/Material.hpp"

namespace FrameEngine {

struct MaterialComponent {

public:
  Material *material = nullptr;

  MaterialComponent(Material *material = nullptr) : material(material) {}
};

} // namespace FrameEngine

#endif // MATERIAL_COMPONENT_HPP
