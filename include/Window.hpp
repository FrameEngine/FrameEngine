#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
  GLFWwindow *handle;

  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);

public:
  Window(int width, int height, const std::string &title);
  ~Window();

  bool isOpen() const;
  void pollEvents() const;
  void swapBuffers() const;
};

#endif // WINDOW_HPP
