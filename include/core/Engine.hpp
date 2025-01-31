#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Registry.hpp"
#include "rendering/RendererAPI.hpp"

class Engine {
private:
  // TODO Specify in the init function, not const
  const float fixedTimeStep = 1.0f / 60.0f;
  float accumulator = 0.0f; // Used to store current dt

protected:
  Registry registry;

public:
  Engine();
  ~Engine();

  void init(); // Bootstrap function
  void run();  // Main loop

  // User can write logic in this functions
  virtual void fixed_update();
  virtual void on_start();
};

#endif // ENGINE_HPP
