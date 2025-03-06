/**
 * @file Window.hpp
 * @brief Provides a window for OpenGL rendering using GLFW.
 *
 * The Window class wraps GLFW functions to create, manage, and update a window.
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
  GLFWwindow *handle; ///< Pointer to the GLFW window.
  int width, height;  ///< Current width and height of the window.

public:
  /**
   * @brief Creates a window with the specified width, height, and title.
   *
   * @param width The width of the window in pixels.
   * @param height The height of the window in pixels.
   * @param title The window title.
   */
  Window(int width, int height, const std::string &title);

  /**
   * @brief Destroys the window and cleans up resources.
   */
  ~Window();

  /**
   * @brief Checks if the window is still open.
   *
   * @return True if the window is open; false otherwise.
   */
  bool isOpen() const;

  /**
   * @brief Polls for and processes all pending window events.
   */
  void pollEvents() const;

  /**
   * @brief Swaps the front and back buffers.
   *
   * Call this after rendering a frame to display the new image.
   */
  void swapBuffers() const;

  /**
   * @brief Gets the current window width.
   *
   * @return The width of the window in pixels.
   */
  int getWidth() const { return width; }

  /**
   * @brief Gets the current window height.
   *
   * @return The height of the window in pixels.
   */
  int getHeight() const { return height; }

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
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
};

#endif // WINDOW_HPP
