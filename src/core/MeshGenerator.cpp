#include "MeshGenerator.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mesh *MeshGenerator::createCube() {
  std::vector<float> vertices = {
      -0.5f, -0.5f, 0.5f,  0.f,  0.f,  1.f, //
      0.5f,  -0.5f, 0.5f,  0.f,  0.f,  1.f, //
      0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f, //

      0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f, //
      -0.5f, 0.5f,  0.5f,  0.f,  0.f,  1.f, //
      -0.5f, -0.5f, 0.5f,  0.f,  0.f,  1.f, //

      -0.5f, -0.5f, -0.5f, 0.f,  0.f,  -1.f, //
      0.5f,  -0.5f, -0.5f, 0.f,  0.f,  -1.f, //
      0.5f,  0.5f,  -0.5f, 0.f,  0.f,  -1.f, //

      0.5f,  0.5f,  -0.5f, 0.f,  0.f,  -1.f, //
      -0.5f, 0.5f,  -0.5f, 0.f,  0.f,  -1.f, //
      -0.5f, -0.5f, -0.5f, 0.f,  0.f,  -1.f, //

      -0.5f, 0.5f,  0.5f,  -1.f, 0.f,  0.f, //
      -0.5f, 0.5f,  -0.5f, -1.f, 0.f,  0.f, //
      -0.5f, -0.5f, -0.5f, -1.f, 0.f,  0.f, //

      -0.5f, -0.5f, -0.5f, -1.f, 0.f,  0.f, //
      -0.5f, -0.5f, 0.5f,  -1.f, 0.f,  0.f, //
      -0.5f, 0.5f,  0.5f,  -1.f, 0.f,  0.f, //

      0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f, //
      0.5f,  0.5f,  -0.5f, 1.f,  0.f,  0.f, //
      0.5f,  -0.5f, -0.5f, 1.f,  0.f,  0.f, //

      0.5f,  -0.5f, -0.5f, 1.f,  0.f,  0.f, //
      0.5f,  -0.5f, 0.5f,  1.f,  0.f,  0.f, //
      0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f, //

      -0.5f, 0.5f,  -0.5f, 0.f,  1.f,  0.f, //
      0.5f,  0.5f,  -0.5f, 0.f,  1.f,  0.f, //
      0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f, //

      0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f, //
      -0.5f, 0.5f,  0.5f,  0.f,  1.f,  0.f, //
      -0.5f, 0.5f,  -0.5f, 0.f,  1.f,  0.f, //

      -0.5f, -0.5f, -0.5f, 0.f,  -1.f, 0.f, //
      0.5f,  -0.5f, -0.5f, 0.f,  -1.f, 0.f, //
      0.5f,  -0.5f, 0.5f,  0.f,  -1.f, 0.f, //

      0.5f,  -0.5f, 0.5f,  0.f,  -1.f, 0.f, //
      -0.5f, -0.5f, 0.5f,  0.f,  -1.f, 0.f, //
      -0.5f, -0.5f, -0.5f, 0.f,  -1.f, 0.f, //
  };

  std::vector<float> positions;
  std::vector<float> normals;
  for (size_t i = 0; i < vertices.size(); i += 6) {
    positions.push_back(vertices[i]);
    positions.push_back(vertices[i + 1]);
    positions.push_back(vertices[i + 2]);
    normals.push_back(vertices[i + 3]);
    normals.push_back(vertices[i + 4]);
    normals.push_back(vertices[i + 5]);
  }
  // Create index buffer for 36 vertices (each vertex is unique here)
  std::vector<unsigned int> indices(vertices.size() / 6);
  for (unsigned int i = 0; i < indices.size(); i++) {
    indices[i] = i;
  }
  return new Mesh(positions, normals, indices);
}

Mesh *MeshGenerator::createSphere(unsigned int longitudeSegments,
                                  unsigned int latitudeSegments) {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<unsigned int> indices;

  float radius = 0.5f;
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

  return new Mesh(positions, normals, indices);
}

Mesh *MeshGenerator::createPlane(float width, float height,
                                 unsigned int segments) {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<unsigned int> indices;

  float halfWidth = width / 2.0f;
  float halfHeight = height / 2.0f;

  for (unsigned int y = 0; y <= segments; ++y) {
    float t = static_cast<float>(y) / segments;
    float posY = t * height - halfHeight;
    for (unsigned int x = 0; x <= segments; ++x) {
      float s = static_cast<float>(x) / segments;
      float posX = s * width - halfWidth;
      positions.push_back(posX);
      positions.push_back(0.f);
      positions.push_back(posY);

      normals.push_back(0.f);
      normals.push_back(1.f);
      normals.push_back(0.f);
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

  return new Mesh(positions, normals, indices);
}
