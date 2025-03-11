#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "rendering/Mesh.hpp"

namespace FrameEngine {

struct MeshComponent {
  Mesh *mesh = nullptr;
  bool wireframe = false;

  MeshComponent(Mesh *mesh = nullptr) : mesh(mesh) {}

  /**
   * @brief Enables or disables wireframe rendering for this object.
   *
   * @param enable True to enable wireframe mode; false to disable.
   */
  void setWireframe(bool enable) { wireframe = enable; }

  /**
   * @brief Checks if wireframe rendering is enabled for this object.
   *
   * @return True if wireframe mode is enabled; false otherwise.
   */
  bool isWireframe() const { return wireframe; }
};

} // namespace FrameEngine

#endif // MESH_COMPONENT_HPP
