#include "rendering/Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex> &verts) : vertices(verts) {}

Mesh Mesh::createCube() {
  Mesh cube;

  cube.vertices = {
      Vertex(-0.5f, -0.5f, -0.5f), //
      Vertex(0.5f, -0.5f, -0.5f),  //
      Vertex(0.5f, 0.5f, -0.5f),   //
      Vertex(-0.5f, 0.5f, -0.5f),  //

      Vertex(-0.5f, -0.5f, 0.5f), //
      Vertex(0.5f, -0.5f, 0.5f),  //
      Vertex(0.5f, 0.5f, 0.5f),   //
      Vertex(-0.5f, 0.5f, 0.5f)   //
  };

  cube.edges = {
      {0, 1}, {1, 2}, {2, 3}, {3, 0}, //
      {4, 5}, {5, 6}, {6, 7}, {7, 4}, //
      {0, 4}, {1, 5}, {2, 6}, {3, 7}  //
  };

  cube.faces = {
      {0, 1, 2, 3}, {4, 5, 6, 7}, //
      {0, 1, 5, 4}, {2, 3, 7, 6}, //
      {1, 2, 6, 5}, {0, 3, 7, 4}  //
  };

  return cube;
}

Mesh Mesh::createSphere(int stacks, int slices, float radius) {
  Mesh sphere;
  float pi = 3.14159265359f;

  // Generate vertices
  for (int i = 0; i <= stacks; ++i) {
    float phi = pi * (-0.5f + (float)i / stacks);
    float y = radius * std::sin(phi);
    float r = radius * std::cos(phi);

    for (int j = 0; j <= slices; ++j) {
      float theta = 2.0f * pi * ((float)j / slices);
      float x = r * std::cos(theta);
      float z = r * std::sin(theta);

      sphere.vertices.push_back(Vertex(x, y, z));
    }
  }

  // Generate edges
  for (int i = 0; i < stacks; ++i) {
    for (int j = 0; j < slices; ++j) {
      int first = i * (slices + 1) + j;
      int second = first + slices + 1;

      sphere.edges.push_back({first, first + 1});
      sphere.edges.push_back({first, second});
    }
  }

  // Generate faces
  for (int i = 0; i < stacks; ++i) {
    for (int j = 0; j < slices; ++j) {
      int first = i * (slices + 1) + j;
      int second = first + slices + 1;

      sphere.faces.push_back({first, first + 1, second + 1, second});
    }
  }

  return sphere;
}
