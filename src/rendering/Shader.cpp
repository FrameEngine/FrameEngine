#include "rendering/Shader.hpp"
#include "Logger.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace FrameEngine {

/**
 * @brief Loads shader source code from a file.
 *
 * @param filePath Path to the shader file.
 * @return A string containing the shader source code.
 */
std::string Shader::loadShaderSource(const std::string &filePath) {
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

/**
 * @brief Compiles a shader from source code.
 *
 * @param source The shader source code.
 * @param type The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
 * @return The compiled shader ID.
 */

GLuint Shader::compileShader(const std::string &source, GLenum type) {
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    LOG(ERROR, "Shader Compilation Error: %s", log);
  }

  return shader;
}

/**
 * @brief Constructs a Shader by loading, compiling, and linking vertex and
 * fragment shaders.
 *
 * @param vertexPath Path to the vertex shader file.
 * @param fragmentPath Path to the fragment shader file.
 */
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  std::string vertexSrc = loadShaderSource(vertexPath);
  std::string fragmentSrc = loadShaderSource(fragmentPath);

  GLuint vertexShader = compileShader(vertexSrc, GL_VERTEX_SHADER);
  GLuint fragmentShader = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

  programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  glAttachShader(programID, fragmentShader);
  glLinkProgram(programID);

  int success;
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success) {
    char log[512];
    glGetProgramInfoLog(programID, 512, nullptr, log);
    LOG(ERROR, "Shader Linking Error: %s", log);
  }

  // Shaders are no longer needed once linked
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

/**
 * @brief Destructor that deletes the shader program.
 */
Shader::~Shader() { glDeleteProgram(programID); }

/**
 * @brief Activates the shader program.
 */
void Shader::bind() const { glUseProgram(programID); }

/**
 * @brief Deactivates the shader program.
 */
void Shader::unbind() const { glUseProgram(0); }

/// ===== Uniforms =====
// TODO Surely there is a better way to work with multiple types.
void Shader::setUniformInt(const std::string &name, int value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
    return;
  }
  glUniform1i(location, value);
}

void Shader::setUniformFloat(const std::string &name, float value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
    return;
  }
  glUniform1f(location, value);
}

void Shader::setUniformVec3(const std::string &name,
                            const Vector3 &value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
    return;
  }
  glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setUniformMat4(const std::string &name, const Matrix4 &mat) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name);
  }
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat.m[0][0]);
}

} // namespace FrameEngine
