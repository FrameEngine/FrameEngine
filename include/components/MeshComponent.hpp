#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "math/Vector3.hpp"
#include <vector>

struct MeshComponent {
  std::vector<Vector3> vertices; // List of 3D points
  std::vector<std::pair<int, int>>
      edges; // Pairs of vertex indices defining edges
};

#endif // MESH_COMPONENT_HPP
