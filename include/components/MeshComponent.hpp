#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "math/Quaternion.hpp"
#include "math/Vector3.hpp"
#include <vector>

struct MeshComponent {
  // List of 3D points
  // Pairs of vertex indices defining edges
  std::vector<Vector3> vertices;
  std::vector<std::pair<int, int>> edges;

  Quaternion rotation = Quaternion(); // Quaternion for rotation
};

#endif // MESH_COMPONENT_HPP
