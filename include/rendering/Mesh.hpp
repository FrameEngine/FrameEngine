#ifndef MESH_HPP
#define MESH_HPP

#include "math/Vector3.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>

class Mesh {
private:
  GLuint VAO, VBO, EBO;
  int indexCount;
  std::vector<float>
      vertexData; // vertexData layout: [pos, pos, pos, norm, norm, norm]

public:
  Mesh(const std::vector<float> &vertices, const std::vector<float> &normals,
       const std::vector<unsigned int> &indices);
  ~Mesh();

  void draw() const;

  std::vector<float> &getVertexData() { return vertexData; }
  GLuint getVAO() const { return VAO; }
  GLuint getVBO() const { return VBO; }

  void updateVertexPosition(unsigned int vertexIndex,
                            const Vector3 &newPosition);

  void updateBuffer();

  static Mesh *loadFromOBJ(const std::string &filePath);
};

#endif // MESH_HPP
