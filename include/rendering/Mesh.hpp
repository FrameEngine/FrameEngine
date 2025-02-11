#ifndef MESH_HPP
#define MESH_HPP

#include "math/Vector3.hpp"
#include <glad/glad.h>

class Mesh {
private:
  GLuint VAO, VBO, EBO;
  Vector3 color;
  int vertexCount, indexCount;

public:
  Mesh(float *vertices, unsigned int *indices, unsigned int numVertices,
       unsigned int numIndices, const Vector3 &color);
  ~Mesh();

  void draw() const;
  Vector3 getColor() const;

  GLuint getVAO() const { return VAO; }
  GLuint getVBO() const { return VBO; }
};

#endif // MESH_HPP
