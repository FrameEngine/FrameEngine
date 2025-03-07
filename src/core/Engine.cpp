/**
 * @file Engine.hpp
 * @brief Defines the core Engine class responsible for running simulation.
 */

#include "Engine.hpp"
#include <chrono>
#include <thread>

Engine::Engine()
    : window(1920, 1080, "FrameEngine"), isRunning(true), renderer() {}

Engine::~Engine() { stop(); }

/**
 * @brief Bootstrap
 * This function should be called before 'run()' to ensure all components
 * are properly set up (e.g., renderer, ECS initialization, etc).
 */
void Engine::init() { on_start(); }

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

  while (isRunning && window.isOpen()) {
    auto currentTime = clock::now();
    float deltaTime =
        std::chrono::duration<float>(currentTime - previousTime).count();
    previousTime = currentTime;

    // Prevent excessive time jumps (spiral of death)
    if (deltaTime > fixedTimeStep * 5) {
      deltaTime = fixedTimeStep * 5;
    }

    accumulator += deltaTime;
    while (accumulator >= fixedTimeStep) {
      fixed_update(fixedTimeStep);
      accumulator -= fixedTimeStep;
    }

    renderer.clear();
    renderer.render();

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
