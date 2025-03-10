#ifndef MESH_GENERATOR_HPP
#define MESH_GENERATOR_HPP

#include "rendering/Mesh.hpp"
#include <vector>

namespace FrameEngine {

class MeshGenerator {
public:
  static Mesh *createCube();

  static Mesh *createSphere(unsigned int longitudeSegments = 16,
                            unsigned int latitudeSegments = 16);

  static Mesh *createPlane(float width = 1.0f, float height = 1.0f,
                           unsigned int segments = 1);
};

} // namespace FrameEngine

#endif // MESH_GENERATOR_HPP
