#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glad/glad.h>
#include <string>

namespace FrameEngine {

class Texture2D {
private:
  GLuint textureID;
  int width, height, channels;

public:
  Texture2D();
  ~Texture2D();

  // Load a texture from an image file
  bool loadFromFile(const std::string &filePath, bool flipVertically = true);

  // Generate a procedural texture (for example, a simple noise texture)
  bool generateProcedural(int w, int h);

  bool createDefaultWhiteTexture();

  // Bind and unbind texture
  void bind(GLenum textureUnit = GL_TEXTURE0) const;
  void unbind() const;

  GLuint getID() const { return textureID; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
};

} // namespace FrameEngine

#endif // TEXTURE2D_HPP
