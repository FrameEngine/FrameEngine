#include "rendering/Mesh.hpp"
#include <vector>

namespace FrameEngine {

/**
 * @brief Constructs a Mesh with given vertices, normals, texCoords, and
indices.
 *
 * This constructor merges the vertex positions, normals, and texture
coordinates (UVs) into one array and uploads the data to the GPU.
 *
 * Each vertex is represented by 8 floats in the following order:
 * - 3 floats for position,
 * - 3 floats for normal,
 * - 2 floats for texture coordinates (UVs).
 *
 * @param positions A vector of vertex positions (3 floats per vertex).
 * @param normals A vector of vertex normals (3 floats per vertex).
 * @param texCoords A vector of texture coordinates (2 floats per vertex).
 * @param indices A vector of indices for drawing the mesh.
 */
Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<float> &normals,
           const std::vector<float> &texCoords,
           const std::vector<unsigned int> &indices)
    : indexCount(static_cast<int>(indices.size())) {

  size_t vertexCount = vertices.size() / 3;

  if (normals.size() / 3 != vertexCount ||
      texCoords.size() / 2 != vertexCount) {
    throw std::runtime_error(
        "Mismatch in vertex attribute sizes: positions, normals, or texCoords");
  }

  vertexData.clear();

  for (size_t i = 0; i < vertexCount; i++) {
    vertexData.push_back(vertices[i * 3 + 0]);
    vertexData.push_back(vertices[i * 3 + 1]);
    vertexData.push_back(vertices[i * 3 + 2]);

    vertexData.push_back(normals[i * 3 + 0]);
    vertexData.push_back(normals[i * 3 + 1]);
    vertexData.push_back(normals[i * 3 + 2]);

    vertexData.push_back(texCoords[i * 2 + 0]);
    vertexData.push_back(texCoords[i * 2 + 1]);
  }

  // Generate and bind the VAO and buffers.
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // Bind and upload the merged vertex data to the VBO.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
               vertexData.data(), GL_DYNAMIC_DRAW);

  // Set vertex attribute pointers:
  // Attribute 0: Position (3 floats), stride = 8 floats, offset = 0.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Attribute 1: Normal (3 floats), stride = 8 floats, offset = 3 *
  // sizeof(float).
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Attribute 2: Texture Coordinates (UVs) (2 floats), stride = 8 floats,
  // offset = 6 * sizeof(float).
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Bind EBO and upload index data.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // Unbind to prevent accidental modifications.
  glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<float> &normals,
           const std::vector<unsigned int> &indices)
    // Generate a default UV array (all zeros) for each vertex.
    // There are 2 UV floats per vertex, so size = (number of vertices * 2)
    : Mesh(vertices, normals, std::vector<float>(vertices.size() / 3 * 2, 0.0f),
           indices) {
  // This constructor delegates to the primary constructor.
}

/**
 * @brief Destructor that releases the OpenGL resources.
 */
Mesh::~Mesh() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

/**
 * @brief Draws the mesh.
 *
 * Binds the VAO and calls glDrawElements to render the mesh.
 */
void Mesh::draw() const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

/**
 * @brief Updates the position of a vertex in the mesh.
 *
 * Changes the position of the vertex at the given index and then updates the
 * GPU buffer.
 *
 * @param vertexIndex The index of the vertex to update.
 * @param newPosition The new position for the vertex.
 */
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

/**
 * @brief Re-uploads the vertex data to the GPU.
 *
 * This should be called after modifying the vertex data.
 */
void Mesh::updateBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(float),
                  vertexData.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace FrameEngine
