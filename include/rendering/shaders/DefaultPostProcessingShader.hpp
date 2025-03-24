#ifndef DEFAULT_POSTPROCESSING_SHADER_HPP
#define DEFAULT_POSTPROCESSING_SHADER_HPP

namespace FrameEngine {
namespace DefaultPostProcessingShader {

// A minimal full–screen quad vertex shader.
static const char *quadVertexShaderSource = R"glsl(
#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)glsl";

// A minimal fragment shader that simply samples a texture.
static const char *screenFragmentShaderSource = R"glsl(
#version 450 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D screenTexture;
void main()
{
    FragColor = texture(screenTexture, TexCoords);
}
)glsl";

} // namespace DefaultPostProcessingShader
} // namespace FrameEngine

#endif // DEFAULT_POSTPROCESSING_SHADERS_HPP
