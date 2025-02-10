#include "Window.hpp"
#include <iostream>

Window::Window(int width, int height, const std::string &title) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!handle) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(handle);
  glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }

  std::cout << "Window successfully created!" << std::endl;
  glViewport(0, 0, width, height);
}

Window::~Window() {
  glfwDestroyWindow(handle);
  glfwTerminate();
}

bool Window::isOpen() const { return !glfwWindowShouldClose(handle); }

void Window::pollEvents() const { glfwPollEvents(); }

void Window::swapBuffers() const { glfwSwapBuffers(handle); }

void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  glViewport(0, 0, width, height);
}
