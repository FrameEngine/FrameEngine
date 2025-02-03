# FrameEngine - physics engine  

I love doing physics simulations and math-related projects! However, rewriting window management, UI, objects, and other related things every time can be tedious. So I decided to build my own engine

<div align="center">
<img src="https://github.com/user-attachments/assets/ccf7c572-5bd8-4c58-899f-363636e1946a" alt="Demo">
</div>

---

## Overview

The core of **FrameEngine** is built around [ECS](https://en.wikipedia.org/wiki/Entity_component_system)(Entity-component-system) and includes:
- **Registry**: A simple ECS `Registry` that allows creating entities and attaching components.
- **Components**: Data containers that define behaviors or properties such as transforms, physics, or rendering details.
- **Systems**: Logic units that iterate through all entities with specific components. For instance, the `PhysicsSystem` handles velocity and position updates, while the `RenderSystem` renderes meshes
- **Engine**: A base `Engine` class that provides a main loop with a **fixed timestep** for physics and a simple rendering pass.

Currently, **X11** rendering is implemented (via `X11RendererAdapter`), though you can add your own (e.g. OpenGL, Vulkan or even selfmade one) by implementing the `IRenderer` interface. (Instructions for doing so can be found in the [Contributing](#Contributing) section)

---

## Getting Started

### Prerequisites

- A **C++17** compiler
- **CMake** >= 3.10
- **Make** (or Ninja, etc., whichever you prefer)
- **X11** development libraries (required for running the X11 example; future versions may use OpenGL)
- **Catch2** test library (for running tests)

On many Linux distributions, you can install these with package managers 
`sudo apt-get install libx11-dev cmake build-essential`

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

Include the FrameEngine directory in your project and update your `CMakeLists.txt` as follows:
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
       // Add your initialization logic here
     }

     void fixed_update(float dt) override {
       // Add your physics logic here
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

We welcome contributions 🎉

- Please view the [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines on submitting issues, pull requests, and coding standards.
- If you’re adding a **renderer**, check out [renderer adapter guidelines ](./include/rendering/adapters/README.md) before you start

Thank you for helping improve this project!

> If you have any questions you can **start a disscusion** or PM me directly on [my socials](https://github.com/Perchinka)

---

## Future Plans

- **OpenGL Renderer** or **Vulkan Renderer** for real 3D drawing.
- Add a more comprehensive **Camera** system.
- Extend the **PhysicsSystem** with collision detection, constraints etc.
- A **GUI** for runtime parameter tweaking.
- **Wiki** and **more comprehensive documentation**

---

## License

This project is licensed under the [MIT License](LICENSE). 
