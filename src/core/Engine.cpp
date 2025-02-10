/**
 * @file Engine.hpp
 * @brief Defines the core Engine class responsible for running simulation.
 */

#include "Engine.hpp"
#include <chrono>
#include <iostream>
#include <thread>

Engine::Engine() : window(800, 600, "FrameEngine"), isRunning(true) {}

Engine::~Engine() { stop(); }

/**
 * @brief Bootstrap
 * This function should be called before 'run()' to ensure all components
 * are properly set up (e.g., renderer, ECS initialization, etc).
 */
void Engine::init() {
  Renderer::initialize();
  on_start();
}

/**
 * @brief Stops the engine gracefully.
 *
 * Not really so far, but will fix it later
 */
void Engine::stop() { isRunning = false; }

// Default implementation (empty), user should override these
void Engine::on_start() {}

void Engine::fixed_update(float dt) {}

/**
 * @brief Starts the main game loop.
 *
 * - Calls on_start()
 * The loop:
 * - Calls 'fixed_update(float dt)'
 * - Calls 'RenderSystem::render()'
 */
void Engine::run() {
  using clock = std::chrono::high_resolution_clock;
  auto previousTime = clock::now();
  float accumulator = 0.0f;

  on_start();

  while (isRunning) {
    auto currentTime = clock::now();
    float deltaTime =
        std::chrono::duration<float>(currentTime - previousTime).count();
    previousTime = currentTime;

    // Prevent excessive time jumps (spiral of death)
    if (deltaTime > 0.1f) {
      deltaTime = 0.1f;
    }

    accumulator += deltaTime;
    while (accumulator >= fixedTimeStep) {
      fixed_update(fixedTimeStep);
      accumulator -= fixedTimeStep;
    }

    Renderer::clear();
    Renderer::present();

    window.swapBuffers();
    window.pollEvents();

    // Compute time left in the frame
    auto frameEnd = clock::now();
    float frameElapsed =
        std::chrono::duration<float>(frameEnd - currentTime).count();

    // If frame finished early, sleep to limit CPU usage
    if (frameElapsed < fixedTimeStep) {
      std::this_thread::sleep_for(
          std::chrono::duration<float>(fixedTimeStep - frameElapsed));
    }
  }
}
