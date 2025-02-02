/**
 * @file MeshComponent.hpp
 * @brief Defines the MeshComponent for ECS-based rendering.
 *
 * The `MeshComponent` is used in the ECS to store and manage an entity's mesh.
 */

#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "rendering/Mesh.hpp"

/**
 * @struct MeshComponent
 * @brief Stores the mesh data associated with an entity.
 *
 * This component is attached to an entity that requires rendering.
 * It holds a `Mesh` object, which contains the geometry and vertex data.
 */
struct MeshComponent {
  Mesh mesh;

  /**
   * @brief Default constructor.
   *
   * The default constructor does not initialize a mesh, allowing
   * the component to be assigned one later.
   */
  MeshComponent() = default;

  /**
   * @brief Constructs a MeshComponent with a specified mesh.
   * @param mesh The mesh to associate with this entity.
   */
  explicit MeshComponent(const Mesh &mesh) : mesh(mesh) {}
};

#endif // MESH_COMPONENT_HPP
