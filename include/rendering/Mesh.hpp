#ifndef MESH_HPP
#define MESH_HPP

#include "math/Vector3.hpp"
#include <glad/glad.h>

class Mesh {
private:
  GLuint VAO, VBO, EBO;
  int vertexCount, indexCount;

public:
  Mesh(float *vertices, float *normals, unsigned int *indices,
       unsigned int numVertices, unsigned int numIndices);
  ~Mesh();

  void draw() const;

  GLuint getVAO() const { return VAO; }
  GLuint getVBO() const { return VBO; }
};

#endif // MESH_HPP
