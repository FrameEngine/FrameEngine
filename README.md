# FrameEngine - physics engine  

I love doing physics simulations and math-related stuff, it's fun, but rewriting window management, ui, objects and other stuff each time is kinda borring. So I decided to make my own engine, and also practice my C++ and general project maintaining skills. 
<div align="center">
<img src="https://github.com/user-attachments/assets/ccf7c572-5bd8-4c58-899f-363636e1946a" alt="Demo">
</div>

---

## Overview

The core of **FrameEngine** is built around ECS(Entity-component-system) and include:
- **Registry**: A simple ECS `Registry` that allows creating entities and attaching components.
- **Components**: Data containers that define behaviors or properties like transforms, physics, or rendering details.
- **Systems**: Logic units that iterate through all entities with certain components. For instance, now the `PhysicsSystem` handles velocity and position updates, while the `RenderSystem` renderes meshes
- **Engine**: A base `Engine` class that provides a main loop with a **fixed timestep** for physics and a simple rendering pass.

Currently, **X11** rendering is implemented (via `X11RendererAdapter`), though you can add your own (e.g. OpenGL, Vulkan) by implementing the `IRenderer` interface. (How to do it you can find in [Contributing](#Contributing) section)

---

## Getting Started

### Prerequisites

- A **C++17** compiler
- **CMake** >= 3.10
- **Make** (or Ninja, etc., whichever you prefer)
- **X11** development libraries (if you want to run the X11 example, in the future will switch to OpenGL)
- **Catch2** test library (for running unit tests)

On many Linux distributions, you can install these with package managers (e.g., `sudo apt-get install libx11-dev cmake build-essential`).

### How to install?

Clone this repository:

```bash
git clone https://github.com/Perchinka/FrameEngine.git
cd FrameEngine
make build
```
After successful compilation, the `build` folder will contain the executables.

### Running tests

To run tests and check if everything was installed correctly:
```bash
make test
```

### Running demo simulation

The demo application is located at `src/main.cpp`. Once built, you can run:
```bash
make run
```
or manually:
```bash
./build/frame_engine
```

This launches a simple **X11** window showing a rotating cube with orbiting spheres.  

Press `Ctrl+C` in the terminal to terminate (or just close the X11 window).

---

## How to use the engine?

Copy this directory into your project and include FrameEngine in your CMakeLists:
```
cmake_minimum_required(VERSION 3.10)
project(MySimulation)

set(CMAKE_CXX_STANDARD 17)

# Add the FrameEngine directory
add_subdirectory(FrameEngine)

# Build your own simulation executable
add_executable(my_sim main.cpp)

# Link against the FrameEngine library
target_link_libraries(my_sim PRIVATE frame_engine)

```

Now you can use **FrameEngine**

1. **Include** the engine headers in your project:
   ```cpp
   #include "FrameEngine.hpp"
   ```

2. **Inherit** from `Engine` to create your own simulation/game class:
   ```cpp
   class MySimulation : public Engine {
     void on_start() override {
       // Add your objects/entities here
     }

     void fixed_update(float dt) override {
       // Add your physics or custom logic here
     }
   };
   ```

3. **Choose a Renderer** by implementing `IRenderer` or using the provided `X11RendererAdapter`, then set it:
   ```cpp

   int main() {
     X11RendererAdapter x11Renderer;
     Renderer::set_renderer(&x11Renderer);
     Renderer::init();

     MySimulation sim;
     sim.init();
     sim.run(); // main loop

     return 0;
   }
   ```

4. **Compile & Run** to see your simulation in action.

---

## Contributing

1. **Fork** the repo and create a new branch for your feature. (feature-xyz)
2. Implement changes or new features.
3. **Add tests** (where it makes sense :D) in the `tests/` directory.
4. Submit a **Pull Request**.

If you’re adding a **new renderer**, see [`include/rendering/adapters/README.md`](./include/rendering/adapters/README.md) for guidelines.

---

## Future Plans

- **OpenGL Renderer** or **Vulkan Renderer** for real 3D drawing.
- Add a more comprehensive **Camera** system.
- Extend the **PhysicsSystem** with collision detection, constraints, etc.
- A **GUI** for runtime parameter tweaking.

---

## License

This project is licensed under the [MIT License](LICENSE). 
