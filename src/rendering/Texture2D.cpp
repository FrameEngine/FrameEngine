#include "rendering/Texture2D.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "Logger.hpp"
#include "stb_image.h"

namespace FrameEngine {

Texture2D::Texture2D() { glGenTextures(1, &textureID); }

Texture2D::~Texture2D() { glDeleteTextures(1, &textureID); }

bool Texture2D::loadFromFile(const std::string &filePath, bool flipVertically) {
  stbi_set_flip_vertically_on_load(flipVertically);
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
  if (!data) {
    LOG(ERROR, "Failed to load texture: %s", filePath.c_str());
    return false;
  }

  GLenum format;
  if (nrChannels == 1)
    format = GL_RED;
  else if (nrChannels == 3)
    format = GL_RGB;
  else if (nrChannels == 4)
    format = GL_RGBA;
  else {
    LOG(WARNING, "Texture format not supported: %d channels", nrChannels);
    stbi_image_free(data);
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);
  return true;
}

bool Texture2D::createDefaultWhiteTexture() {
  unsigned char whitePixel[4] = {255, 255, 255, 255};
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               whitePixel);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

void Texture2D::bind(GLenum textureUnit) const {
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace FrameEngine
