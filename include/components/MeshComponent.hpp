#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"
#include <vector>

struct MeshComponent {
  std::vector<Vector3> vertices; // Local mesh
  std::vector<std::pair<int, int>>
      edges; // Pairs of vertex indices defining edges
  std::vector<Vector3> transformedVertices; // Global verticies
  //
  MeshComponent() = default;
  MeshComponent(std::vector<Vector3> verts,
                std::vector<std::pair<int, int>> eds)
      : vertices(std::move(verts)), edges(std::move(eds)),
        transformedVertices(vertices) {}
};

#endif // MESH_COMPONENT_HPP
