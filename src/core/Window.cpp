/**
 * @file Window.hpp
 * @brief Provides a window for OpenGL rendering using GLFW.
 *
 * The Window class wraps GLFW functions to create, manage, and update a window.
 */
#include "Window.hpp"
#include "Logger.hpp"

namespace FrameEngine {

/**
 * @brief Creates a window with the specified width, height, and title.
 *
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @param title The window title.
 */

Window::Window(int width, int height, const std::string &title)
    : width(width), height(height) {
  if (!glfwInit()) {
    LOG(CRITICAL, "Failed to initialize GLFW");
    exit(-1);
  }

  // Set openGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create the GLFW window.
  handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!handle) {
    LOG(CRITICAL, "Failed to create GLFW window");
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(handle);

  // Register the framebuffer size callback.
  glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);

  // Load OpenGL functions using GLAD.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG(CRITICAL, "Failed to initialize GLAD");
    exit(-1);
  }

  LOG(INFO, "Window successfully created!");
  glViewport(0, 0, width, height);
}

/**
 * @brief Destroys the window and cleans up resources.
 */
Window::~Window() {
  glfwDestroyWindow(handle);
  glfwTerminate();
}

/**
 * @brief Checks if the window is still open.
 *
 * @return True if the window is open; false otherwise.
 */
bool Window::isOpen() const { return !glfwWindowShouldClose(handle); }

/**
 * @brief Polls for and processes all pending window events.
 */
void Window::pollEvents() const { glfwPollEvents(); }

/**
 * @brief Swaps the front and back buffers.
 *
 * Call this after rendering a frame to display the new image.
 */
void Window::swapBuffers() const { glfwSwapBuffers(handle); }

/**
 * @brief Callback function that updates the viewport when the window is
 * resized.
 *
 * This function is called automatically by GLFW when the window size changes.
 * Needed to handle aspec ratio properly.
 *
 * @param window The GLFW window.
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void Window::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  glViewport(0, 0, width, height);

  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (win) {
    win->width = width;
    win->height = height;
  }
}

} // namespace FrameEngine
