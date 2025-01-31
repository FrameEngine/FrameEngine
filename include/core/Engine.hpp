#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "rendering/RendererAPI.hpp"

class Engine {
private:
  // TODO Specify in the init function, not const
  const float fixedTimeStep = 1.0f / 60.0f;
  float accumulator = 0.0f; // Used to store current dt

public:
  Engine();
  ~Engine();

  void init(); // Bootstrap function
  void run();  // Main loop

  // User can write logic in this function outside engine code
  void fixed_update();

  void render();
};

#endif // ENGINE_HPP
