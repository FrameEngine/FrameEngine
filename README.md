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

FrameEngine currently supports **OpenGL rendering** using **X11** as the windowing backend.  
Currently, it runs on **Linux only**, but we plan to add cross-platform support soon.

---

## Getting Started

### Prerequisites

- A **C++17** compiler
- **CMake** >= 3.10
- **Make**
- **Catch2** unit testing library
- **X11-dev libraries** so far we use x11 as our window-backend
- **Doxygen** auto-generated documentation

On many Linux systems, you can install these with package managers like `apt`, e.g:
```bash 
sudo apt-get install -y \
              libx11-dev \
              libxext-dev \
              libxrandr-dev \
              libxinerama-dev \
              libxcursor-dev \
              libxi-dev \
              libgl1-mesa-dev pkg-config doxygen graphviz
```

### Building the Engine

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

### Running the demo

The demo application is located at `src/main.cpp`. Once built, you can run:
```bash
make run
```
or manually:
```bash
./build/frame_engine
```

This will launch a simple OpenGL window with a demo scene
Press `Ctrl+C` in the terminal to terminate (or just close the window).

---

## How to use the engine?

Include the FrameEngine directory in your project and update your `CMakeLists.txt` as follows:
```
cmake_minimum_required(VERSION 3.10)
project(MySimulation)

set(CMAKE_CXX_STANDARD 17)

# Include the FrameEngine directory
include_directories(FrameEngine/include)

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

3. **Init and Run**
   ```cpp

   int main() {
      Simulation sim;
      sim.init();
      sim.run();
      return 0;

     return 0;
   }
   ```

---

## Contributing

We welcome contributions 🎉

Please view the [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines on submitting issues, pull requests, and coding standards.

Thank you for helping improve this project!

> If you have any questions you can **start a disscusion** or PM me directly on [my socials](https://github.com/Perchinka)

---

## Future Plans
We're continuously improving FrameEngine. Follow our **[roadmap](https://github.com/users/Perchinka/projects/9)** for upcoming features and enhancements

**Have feature suggestions or want to contribute?** Feel free to start a discussion or check out our [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines.

---

## License

This project is licensed under the [MIT License](./LICENSE). 
