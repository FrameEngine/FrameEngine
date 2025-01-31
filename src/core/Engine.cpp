#include "core/Engine.hpp"
#include "chrono"
#include "thread"

Engine::Engine() {}

Engine::~Engine() {}

// Bootstrap
void Engine::init() { RendererAPI::init(); }

void Engine::run() {
  using clock = std::chrono::high_resolution_clock;
  auto previousTime = clock::now();
  float accumulator = 0.0f;

  on_start();

  while (true) {
    auto currentTime = clock::now();
    float deltaTime =
        std::chrono::duration<float>(currentTime - previousTime).count();
    previousTime = currentTime;

    // Prevent spiral of death
    if (deltaTime > 0.1f) {
      deltaTime = 0.1f; // TODO make this variable
    }

    accumulator += deltaTime;

    // Fixed time step updates
    while (accumulator >= fixedTimeStep) {
      fixed_update();
      accumulator -= fixedTimeStep;
    }

    // Compute how much time is left in the frame
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
