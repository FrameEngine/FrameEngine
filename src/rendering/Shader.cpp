#include "rendering/Shader.hpp"
#include "Logger.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace FrameEngine {

std::string Shader::loadShaderSource(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    LOG(ERROR, "Failed to open shader file: %s", filePath.c_str());
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint Shader::compileShader(const std::string &source, GLenum type) {
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    LOG(ERROR, "Shader Compilation Error: %s", infoLog);
  }
  return shader;
}

GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    LOG(ERROR, "Shader Linking Error: %s", infoLog);
  }
  // After linking, shaders can be deleted.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return program;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  // Load shader source code from files.
  std::string vertexCode = loadShaderSource(vertexPath);
  std::string fragmentCode = loadShaderSource(fragmentPath);
  if (vertexCode.empty() || fragmentCode.empty()) {
    LOG(ERROR, "Shader source empty. Check file paths.");
  }
  GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
  GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
  programID = linkProgram(vertexShader, fragmentShader);
}

Shader::Shader(const std::string &vertexSource,
               const std::string &fragmentSource, bool fromSource) {
  // The parameter 'fromSource' is ignored if false.
  if (!fromSource) {

  } else {
    // Use the provided strings as the shader source code.
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    programID = linkProgram(vertexShader, fragmentShader);
  }
}

Shader::~Shader() { glDeleteProgram(programID); }

void Shader::bind() const { glUseProgram(programID); }

void Shader::unbind() const { glUseProgram(0); }

void Shader::setUniformInt(const std::string &name, int value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name.c_str());
    return;
  }
  glUniform1i(location, value);
}

void Shader::setUniformFloat(const std::string &name, float value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name.c_str());
    return;
  }
  glUniform1f(location, value);
}

void Shader::setUniformVec3(const std::string &name,
                            const Vector3 &value) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name.c_str());
    return;
  }
  glUniform3f(location, static_cast<GLfloat>(value.x),
              static_cast<GLfloat>(value.y), static_cast<GLfloat>(value.z));
}

void Shader::setUniformMat4(const std::string &name, const Matrix4 &mat) const {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if (location == -1) {
    LOG(WARNING, "Uniform '%s' not found!", name.c_str());
  }
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat.m[0][0]);
}

} // namespace FrameEngine
