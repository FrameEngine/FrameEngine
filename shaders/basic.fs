#version 450 core
#define MAX_LIGHTS 10

struct LightData {
    vec4 position;   // xyz: world-space position 
    vec4 color;      // rgb: light color, w: intensity
};

layout(std140, binding = 0) uniform LightBlock {
    int numLights;
    LightData lights[MAX_LIGHTS];
};

uniform vec3 ambientColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float specularPower;
    
    int emissiveEnabled;
    vec3 emissiveColor;
};
uniform Material material;

uniform sampler2D texSampler;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 ambient = ambientColor * material.diffuseColor;
    
    vec3 diffuseTotal = vec3(0.0);
    vec3 specularTotal = vec3(0.0);
    
    for (int i = 0; i < numLights; i++) {
        vec3 lightPos = lights[i].position.xyz;
        vec3 lightColor = lights[i].color.rgb;
        float intensity = lights[i].color.w;
        
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * material.diffuseColor;
        
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = 0.0;
        if (diff > 0.0) {
            spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularPower);
        }
        vec3 specular = spec * lightColor * material.specularColor;
        
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
        
        diffuseTotal += diffuse * intensity * attenuation;
        specularTotal += specular * intensity * attenuation;
    }
    
    vec3 texColor = texture(texSampler, TexCoords).rgb;
    vec3 finalColor = (ambient + diffuseTotal + specularTotal) * texColor;
    
    if (material.emissiveEnabled == 1) {
        finalColor += material.emissiveColor;
    }
    
    FragColor = vec4(finalColor, 1.0);
}
