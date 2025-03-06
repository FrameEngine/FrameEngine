/**
 * @file Shader.hpp
 * @brief Manages OpenGL shaders.
 *
 * The Shader class loads, compiles, and links shaders from
 * files. It provides helper methods for binding the shader program and setting
 * uniform variables.
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include "math/Matrix4.hpp"
#include "math/Vector3.hpp"
#include <glad/glad.h>
#include <string>

class Shader {
private:
  GLuint programID; ///< The ID of the linked shader program.

  /**
   * @brief Loads shader source code from a file.
   *
   * @param filePath Path to the shader file.
   * @return A string containing the shader source code.
   */
  std::string loadShaderSource(const std::string &filePath);

  /**
   * @brief Compiles a shader from source code.
   *
   * @param source The shader source code.
   * @param type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
   * @return The compiled shader ID.
   */
  GLuint compileShader(const std::string &source, GLenum type);

public:
  /**
   * @brief Constructs a Shader by loading, compiling, and linking vertex and
   * fragment shaders.
   *
   * @param vertexPath Path to the vertex shader file.
   * @param fragmentPath Path to the fragment shader file.
   */
  Shader(const std::string &vertexPath, const std::string &fragmentPath);

  /**
   * @brief Destructor that deletes the shader program.
   */
  ~Shader();

  /**
   * @brief Activates the shader program.
   */
  void bind() const;

  /**
   * @brief Deactivates the shader program.
   */
  void unbind() const;

  /// ===== Uniforms =====
  void setUniformInt(const std::string &name, int value) const;
  void setUniformFloat(const std::string &name, float value) const;
  void setUniformVec3(const std::string &name, const Vector3 &value) const;
  void setUniformMat4(const std::string &name, const Matrix4 &mat) const;
  /// ====================

  /**
   * @brief Returns the shader program ID.
   *
   * @return The OpenGL program ID.
   */
  GLuint getProgramID() const { return programID; }
};

#endif // SHADER_HPP
