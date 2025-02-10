#include "Engine.hpp"

class Simulation : public Engine {
public:
  void on_start() override {}

  void fixed_update(float dt) override {}
};

int main() {
  Simulation sim;
  sim.init();
  sim.run();
  return 0;
}
