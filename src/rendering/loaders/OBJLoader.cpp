#include "Logger.hpp"
#include "rendering/Mesh.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace FrameEngine {

struct UV {
  float u, v;
  UV(float u, float v) : u(u), v(v) {}
  UV() : u(0.0f), v(0.0f) {}
};

Mesh *Mesh::loadFromOBJ(const std::string &filePath) {
  // Temporary containers for raw OBJ data.
  std::vector<Vector3> tempVertices;
  std::vector<UV> tempTexCoords;
  std::vector<Vector3> tempNormals;
  std::vector<unsigned int> indices;

  // Final arrays to be merged into the vertex buffer.
  std::vector<float> vertices;
  std::vector<float> texCoords;
  std::vector<float> normals;

  std::ifstream file(filePath);
  if (!file.is_open()) {
    LOG(ERROR, "Failed to open OBJ file: %s", filePath.c_str());
    return nullptr;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "v") {
      float x, y, z;
      iss >> x >> y >> z;
      tempVertices.push_back(Vector3(x, y, z));
    } else if (prefix == "vt") {
      float u, v;
      iss >> u >> v;
      tempTexCoords.push_back(UV(u, v));
    } else if (prefix == "vn") {
      float x, y, z;
      iss >> x >> y >> z;
      tempNormals.push_back(Vector3(x, y, z));
    } else if (prefix == "f") {
      // Faces can be in two formats:
      // 1) "f v//vn v//vn v//vn" (no texture coordinates)
      // 2) "f v/vt/vn v/vt/vn v/vt/vn" (with texture coordinates)
      //
      // We'll read each vertex of the face as a token.
      for (int i = 0; i < 3; i++) {
        std::string token;
        iss >> token;
        // Find positions of the first and second '/'.
        size_t firstSlash = token.find('/');
        size_t secondSlash = token.find('/', firstSlash + 1);

        // Parse the vertex index.
        unsigned int vIdx = std::stoi(token.substr(0, firstSlash)) - 1;
        unsigned int vtIdx = 0;
        unsigned int nIdx = 0;

        // Check if there is a texture coordinate:
        // Format "v//vn" will have firstSlash immediately followed by another
        // '/'.
        if (firstSlash != std::string::npos &&
            secondSlash != std::string::npos && secondSlash - firstSlash > 1) {
          vtIdx = std::stoi(token.substr(firstSlash + 1,
                                         secondSlash - firstSlash - 1)) -
                  1;
        }

        if (secondSlash != std::string::npos) {
          nIdx = std::stoi(token.substr(secondSlash + 1)) - 1;
        }

        indices.push_back(vertices.size() / 3);

        if (vIdx < tempVertices.size()) {
          vertices.push_back(tempVertices[vIdx].x);
          vertices.push_back(tempVertices[vIdx].y);
          vertices.push_back(tempVertices[vIdx].z);
        } else {
          LOG(ERROR, "Vertex index out of bounds in OBJ file: %s", filePath);
          return nullptr;
        }

        // Append texture coordinate.
        // If tempTexCoords is not empty and the token format indicates a UV,
        // then use it. Otherwise, assign a default value of 0.
        if (!tempTexCoords.empty() && token.find("//") == std::string::npos) {
          if (vtIdx < tempTexCoords.size()) {
            texCoords.push_back(tempTexCoords[vtIdx].u);
            texCoords.push_back(tempTexCoords[vtIdx].v);
          } else {
            // Out-of-bounds: assign default UV.
            texCoords.push_back(0.0f);
            texCoords.push_back(0.0f);
          }
        } else {
          // No UV data present; assign default UV (0,0).
          texCoords.push_back(0.0f);
          texCoords.push_back(0.0f);
        }

        if (nIdx < tempNormals.size()) {
          normals.push_back(tempNormals[nIdx].x);
          normals.push_back(tempNormals[nIdx].y);
          normals.push_back(tempNormals[nIdx].z);
        } else {
          LOG(ERROR, "Normal index out of bounds in OBJ file: %s", filePath);
          return nullptr;
        }
      }
    }
  }

  // Create a new Mesh using the constructor that accepts UVs.
  return new Mesh(vertices, normals, texCoords, indices);
}

} // namespace FrameEngine
