/**
 * @file Mesh.hpp
 * @brief Defines the Mesh class for 3D geometry representation.
 *
 * The Mesh class stores vertices, edges, and faces. Basically define a 3D
 * shape.
 */

#ifndef MESH_HPP
#define MESH_HPP

#include "math/Vector3.hpp"
#include <utility>
#include <vector>

/**
 * @struct Vertex
 * @brief Represents a single vertex in 3D space.
 */
struct Vertex {
  Vector3 position;

  /**
   * @brief Constructs a vertex with the given coordinates.
   * @param x X-coordinate.
   * @param y Y-coordinate.
   * @param z Z-coordinate.
   */
  Vertex(float x, float y, float z) : position(x, y, z) {}
};

/**
 * @class Mesh
 * @brief Represents a 3D shape composed of vertices, edges, and faces.
 *
 * The Mesh class serves as a container for geometric data
 *
 * This class contains some utility functions to draw primitive shapes like
 * cubes and spheres.
 */
class Mesh {
public:
  /// List of vertices.
  std::vector<Vertex> vertices;
  /// List of edges (pairs of vertex indices).
  std::vector<std::pair<int, int>> edges;
  /// List of faces (each face is a list of vertex indices).
  std::vector<std::vector<int>> faces;

  /**
   * @brief Default constructor (creates an empty mesh).
   *
   * The mesh can later be populated manually.
   */
  Mesh() = default;

  /**
   * @brief Constructs a mesh from a given list of vertices.
   * @param verts The list of vertices to initialize the mesh.
   *
   * The mesh is created with only vertices; edges and faces
   * need to be manually defined.
   */
  explicit Mesh(const std::vector<Vertex> &verts);

  // ====== Utility ======

  /**
   * @brief Creates a cube mesh.
   * @return A Mesh representing a cube.
   */
  static Mesh createCube();

  /**
   * @brief Creates a sphere mesh.
   * @param stacks The number of vertical divisions (default: 10).
   * @param slices The number of horizontal divisions (default: 10).
   * @param radius The radius of the sphere (default: 0.5).
   * @return A `Mesh` representing a sphere.
   *
   * The sphere is generated using latitude-longitude subdivision.
   */
  static Mesh createSphere(int stacks = 10, int slices = 10,
                           float radius = 0.5f);
};

#endif // MESH_HPP
