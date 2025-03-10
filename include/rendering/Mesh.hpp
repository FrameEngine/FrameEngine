/**
 * @file Mesh.hpp
 * @brief Defines the Mesh class for managing 3D geometry.
 */
#ifndef MESH_HPP
#define MESH_HPP

#include "math/Vector3.hpp"
#include <glad/glad.h>
#include <string>
#include <vector>

namespace FrameEngine {

class Mesh {
private:
  /// TODO Replace with internal buffers
  /// It's hardcoded, as we plan to use OpenGL, but it will be hard to switch to
  /// something else if we continue using OpenGL-things in domain
  GLuint VAO;     ///< OpenGL Vertex Array Object identifier.
  GLuint VBO;     ///< OpenGL Vertex Buffer Object identifier.
  GLuint EBO;     ///< OpenGL Element Buffer Object identifier.
  int indexCount; ///< Number of indices used for drawing.
  std::vector<float>
      vertexData; ///< The vertex data: [pos, pos, pos, norm, norm, norm]

public:
  /**
   * @brief Constructs a Mesh with given vertices, normals, and indices.
   *
   * The constructor merges the vertex positions and normals into one array
   * and uploads the data to the GPU.
   *
   * @param vertices A vector of vertex positions.
   * @param normals A vector of vertex normals.
   * @param indices A vector of indices for drawing the mesh.
   */
  Mesh(const std::vector<float> &vertices, const std::vector<float> &normals,
       const std::vector<unsigned int> &indices);

  /**
   * @brief Destructor that releases the OpenGL resources.
   */
  ~Mesh();

  /**
   * @brief Draws the mesh.
   *
   * Binds the VAO and calls glDrawElements to render the mesh.
   */
  void draw() const;

  /**
   * @brief Returns a reference to the vertex data.
   *
   * The data layout is: [x, y, z, nx, ny, nz] per vertex.
   *
   * @return A reference to the vector containing the vertex data.
   */
  std::vector<float> &getVertexData() { return vertexData; }

  /**
   * @brief Returns the VAO identifier.
   *
   * @return The OpenGL VAO ID.
   */
  GLuint getVAO() const { return VAO; }

  /**
   * @brief Returns the VBO identifier.
   *
   * @return The OpenGL VBO ID.
   */
  GLuint getVBO() const { return VBO; }

  /**
   * @brief Updates the position of a vertex in the mesh.
   *
   * Changes the position of the vertex at the given index and then updates the
   * GPU buffer.
   *
   * @param vertexIndex The index of the vertex to update.
   * @param newPosition The new position for the vertex.
   */
  void updateVertexPosition(unsigned int vertexIndex,
                            const Vector3 &newPosition);

  /**
   * @brief Re-uploads the vertex data to the GPU.
   *
   * This should be called after modifying the vertex data.
   */
  void updateBuffer();

  /**
   * @brief Loads a mesh from an OBJ file.
   *
   * Extracts data from .OBJ, and creates a Mesh object.
   *
   * @param filePath The path to the OBJ file.
   * @return A pointer to the new Mesh, or nullptr if loading fails.
   */
  static Mesh *loadFromOBJ(const std::string &filePath);
};

} // namespace FrameEngine

#endif // MESH_HPP
