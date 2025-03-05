#include "rendering/Mesh.hpp"
#include <vector>

Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<float> &normals,
           const std::vector<unsigned int> &indices)
    : indexCount(static_cast<int>(indices.size())) {

  // Merge vertices and normals into a single buffer
  vertexData.clear();
  for (size_t i = 0; i < vertices.size() / 3; i++) {
    vertexData.push_back(vertices[i * 3 + 0]);
    vertexData.push_back(vertices[i * 3 + 1]);
    vertexData.push_back(vertices[i * 3 + 2]);
    vertexData.push_back(normals[i * 3 + 0]);
    vertexData.push_back(normals[i * 3 + 1]);
    vertexData.push_back(normals[i * 3 + 2]);
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Use dynamic draw since we plan to update the buffer later
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
               vertexData.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Mesh::draw() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::updateVertexPosition(unsigned int vertexIndex,
                                const Vector3 &newPosition) {
  size_t offset =
      vertexIndex * 6; // 6 floats per vertex (3 for position, 3 for normal)
  if (offset + 2 < vertexData.size()) {
    vertexData[offset + 0] = newPosition.x;
    vertexData[offset + 1] = newPosition.y;
    vertexData[offset + 2] = newPosition.z;

    updateBuffer();
  }
}

void Mesh::updateBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(float),
                  vertexData.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
