# FrameEngine - Rendering Adapter 

Rendering in FrameEngine is abstracted through an **interface (`IRenderer`)**.  
This allows to **switch between different rendering backends** without modifying the core engine.

### **Current Implementations**
- [x] `X11RendererAdapter`
- [] `OpenGLRenderer`
- [] `VulkanRenderer`

---

## How to Build New Renderer
To create a new renderer, follow these steps:

### 1. Create a New Renderer Class  
Your renderer **must** inherit from `IRenderer` and implement all virtual functions.  

#### Example: `MyCustomRenderer.hpp`  
```
#ifndef MY_CUSTOM_RENDERER_HPP  
#define MY_CUSTOM_RENDERER_HPP  

#include "rendering/IRenderer.hpp"  

class MyCustomRenderer : public IRenderer {  
public:  
    MyCustomRenderer();  
    ~MyCustomRenderer();  

    void initialize() override;  
    void clearScreen() override;  
    void drawMesh(const Mesh &mesh, const Matrix4 &transform) override;  
    void swapBuffers() override;  
    void shutdown() override;  
};  

#endif // MY_CUSTOM_RENDERER_HPP  
```

---

### 2. Implement Your Renderer  
You need to define the **core rendering functions** in the `.cpp` file.  

#### Example: `MyCustomRenderer.cpp`  
```
#include "rendering/adapters/MyCustomRenderer.hpp"  
#include <iostream>  

MyCustomRenderer::MyCustomRenderer() {}  

MyCustomRenderer::~MyCustomRenderer() {  
    shutdown();  
}  

void MyCustomRenderer::initialize() {  
    std::cout << "MyCustomRenderer Initialized!" << std::endl;  
}  

void MyCustomRenderer::clearScreen() {  
    std::cout << "Clearing screen..." << std::endl;  
}  

void MyCustomRenderer::drawMesh(const Mesh &mesh, const Matrix4 &transform) {  
    std::cout << "Drawing mesh with custom renderer..." << std::endl;  
}  

void MyCustomRenderer::swapBuffers() {  
    std::cout << "Swapping buffers..." << std::endl;  
}  

void MyCustomRenderer::shutdown() {  
    std::cout << "Shutting down custom renderer..." << std::endl;  
}  

```

---

### 3. Choose Your Renderer in `main.cpp`  
Once implemented, you can **set your custom renderer** in the `main()` function.  

#### Example: `main.cpp`  
```
#include "core/Engine.hpp"  
#include "rendering/adapters/MyCustomRenderer.hpp"  

int main() {  
    MyCustomRenderer customRenderer;  
    Renderer::set_renderer(&customRenderer);  
    Renderer::init();  

    Engine engine;  
    engine.init();  
    engine.run();  

    return 0;  
}  
```
---

## Building and Compiling  
```
make rebuild 
make run
```

---

## Next Steps  
- Implement `OpenGLRenderer`
- Add camera controls for a better 3D experience.  
- Better X11 3d rendering

---

## Need Help?  
If you have any questions, feel free to open an issue or [reach me out](https://github.com/Perchinka)

