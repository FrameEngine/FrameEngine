#include "objects/Object.hpp"
#include "Logger.hpp"
#include "components/MaterialComponent.hpp"
#include "math/Vector3.hpp"
#include "rendering/Renderer.hpp"

namespace FrameEngine {

void Object::setMesh(Mesh *mesh, bool wireframe) {
  if (!get_component<MeshComponent>()) {
    add_component<MeshComponent>(MeshComponent(mesh));
  } else {
    get_component<MeshComponent>()->mesh = mesh;
  }
  get_component<MeshComponent>()->wireframe = wireframe;
}

void Object::setMaterial(Material *material) {
  if (!get_component<MaterialComponent>()) {
    add_component<MaterialComponent>(MaterialComponent(material));
  } else {
    get_component<MaterialComponent>()->material = material;
  }
}

} // namespace FrameEngine
