#include "MeshGenerator.hpp"
#include <cmath>

namespace FrameEngine {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Creates a cube mesh.
 *
 * @return A pointer to a new Mesh object.
 */
Mesh *MeshGenerator::createCube() {
  std::vector<float> vertices = {
      // Front face (z = 0.5), normal (0, 0, 1)
      -0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, //
      0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f,  //
      0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f,   //

      0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f,   //
      -0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.f,  //
      -0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, //

      // Back face (z = -0.5), normal (0, 0, -1)
      -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, //
      0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f,  //
      0.5f, 0.5f, -0.5f, 0.f, 0.f, -1.f,   //

      0.5f, 0.5f, -0.5f, 0.f, 0.f, -1.f,   //
      -0.5f, 0.5f, -0.5f, 0.f, 0.f, -1.f,  //
      -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, //

      // Left face (x = -0.5), normal (-1, 0, 0)
      -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f,   //
      -0.5f, 0.5f, -0.5f, -1.f, 0.f, 0.f,  //
      -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, //

      -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, //
      -0.5f, -0.5f, 0.5f, -1.f, 0.f, 0.f,  //
      -0.5f, 0.5f, 0.5f, -1.f, 0.f, 0.f,   //

      // Right face (x = 0.5), normal (1, 0, 0)
      0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f,   //
      0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f,  //
      0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, //

      0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, //
      0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f,  //
      0.5f, 0.5f, 0.5f, 1.f, 0.f, 0.f,   //

      // Top face (y = 0.5), normal (0, 1, 0)
      -0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, //
      0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f,  //
      0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f,   //

      0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f,   //
      -0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.f,  //
      -0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, //

      // Bottom face (y = -0.5), normal (0, -1, 0)
      -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, //
      0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,  //
      0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f,   //

      0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f,   //
      -0.5f, -0.5f, 0.5f, 0.f, -1.f, 0.f,  //
      -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, //
  };

  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<unsigned int> indices;

  size_t vertexCount = vertices.size() / 6;
  //   Triangle 1: uv0 = (0,0), uv1 = (1,0), uv2 = (1,1)
  //   Triangle 2: uv3 = (1,1), uv4 = (0,1), uv5 = (0,0)
  for (size_t i = 0; i < vertexCount; i++) {
    positions.push_back(vertices[i * 6 + 0]);
    positions.push_back(vertices[i * 6 + 1]);
    positions.push_back(vertices[i * 6 + 2]);

    normals.push_back(vertices[i * 6 + 3]);
    normals.push_back(vertices[i * 6 + 4]);
    normals.push_back(vertices[i * 6 + 5]);

    size_t faceVertexIndex = i % 6;
    if (faceVertexIndex == 0) {
      uvs.push_back(0.0f);
      uvs.push_back(0.0f);
    } else if (faceVertexIndex == 1) {
      uvs.push_back(1.0f);
      uvs.push_back(0.0f);
    } else if (faceVertexIndex == 2) {
      uvs.push_back(1.0f);
      uvs.push_back(1.0f);
    } else if (faceVertexIndex == 3) {
      uvs.push_back(1.0f);
      uvs.push_back(1.0f);
    } else if (faceVertexIndex == 4) {
      uvs.push_back(0.0f);
      uvs.push_back(1.0f);
    } else if (faceVertexIndex == 5) {
      uvs.push_back(0.0f);
      uvs.push_back(0.0f);
    }
    indices.push_back(i);
  }
  return new Mesh(positions, normals, uvs, indices);
}

/**
 * @brief Creates a sphere mesh with spherical UV mapping.
 *
 * UV mapping for a sphere is computed using spherical coordinates.
 * For each vertex:
 *   u = phi / (2*PI)  where phi is the azimuth angle (0 to 2*PI)
 *   v = theta / PI    where theta is the polar angle (0 to PI)
 *
 * @param longitudeSegments Number of segments along the longitude.
 * @param latitudeSegments Number of segments along the latitude.
 * @return A pointer to a new Mesh object.
 */
Mesh *MeshGenerator::createSphere(unsigned int longitudeSegments,
                                  unsigned int latitudeSegments, float radius) {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<unsigned int> indices;

  for (unsigned int y = 0; y <= latitudeSegments; ++y) {
    float v = static_cast<float>(y) / latitudeSegments;
    float theta = v * M_PI;
    for (unsigned int x = 0; x <= longitudeSegments; ++x) {
      float u = static_cast<float>(x) / longitudeSegments;
      float phi = u * 2.0f * M_PI;

      float posX = radius * sin(theta) * cos(phi);
      float posY = radius * cos(theta);
      float posZ = radius * sin(theta) * sin(phi);

      positions.push_back(posX);
      positions.push_back(posY);
      positions.push_back(posZ);

      normals.push_back(posX);
      normals.push_back(posY);
      normals.push_back(posZ);

      uvs.push_back(u);
      uvs.push_back(v);
    }
  }

  for (unsigned int y = 0; y < latitudeSegments; ++y) {
    for (unsigned int x = 0; x < longitudeSegments; ++x) {
      unsigned int first = y * (longitudeSegments + 1) + x;
      unsigned int second = first + longitudeSegments + 1;
      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(first + 1);

      indices.push_back(second);
      indices.push_back(second + 1);
      indices.push_back(first + 1);
    }
  }
  return new Mesh(positions, normals, uvs, indices);
}

/**
 * @brief Creates a plane mesh.
 *
 * The plane is centered at the origin and spans the given width and height.
 * UV coordinates are generated by mapping the plane's X and Z coordinates
 * to the [0, 1] range.
 *
 * For each vertex:
 *   u = (posX + halfWidth) / width
 *   v = (posZ + halfHeight) / height
 *
 * @param width The total width of the plane.
 * @param height The total height of the plane.
 * @param segments The number of subdivisions per edge.
 * @return A pointer to a new Mesh object.
 */
Mesh *MeshGenerator::createPlane(float width, float height,
                                 unsigned int segments) {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;
  std::vector<unsigned int> indices;

  float halfWidth = width / 2.0f;
  float halfHeight = height / 2.0f;

  for (unsigned int y = 0; y <= segments; ++y) {
    float t = static_cast<float>(y) / segments;
    float posZ = t * height - halfHeight;
    for (unsigned int x = 0; x <= segments; ++x) {
      float s = static_cast<float>(x) / segments;
      float posX = s * width - halfWidth;
      positions.push_back(posX);
      positions.push_back(0.f);
      positions.push_back(posZ);

      normals.push_back(0.f);
      normals.push_back(1.f);
      normals.push_back(0.f);

      float u = (posX + halfWidth) / width;
      float v = (posZ + halfHeight) / height;
      uvs.push_back(u);
      uvs.push_back(v);
    }
  }

  for (unsigned int y = 0; y < segments; ++y) {
    for (unsigned int x = 0; x < segments; ++x) {
      unsigned int i0 = y * (segments + 1) + x;
      unsigned int i1 = i0 + 1;
      unsigned int i2 = i0 + (segments + 1);
      unsigned int i3 = i2 + 1;

      indices.push_back(i0);
      indices.push_back(i2);
      indices.push_back(i1);

      indices.push_back(i1);
      indices.push_back(i2);
      indices.push_back(i3);
    }
  }

  return new Mesh(positions, normals, uvs, indices);
}

} // namespace FrameEngine
