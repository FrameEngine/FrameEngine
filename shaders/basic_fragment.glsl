#version 450 core

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float specularPower;
};

uniform Material material;

#define MAX_LIGHTS 10
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform PointLight lights[MAX_LIGHTS];
uniform int numLights;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * material.diffuseColor;
        
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularPower);
        vec3 specular = lights[i].color * material.specularColor * spec * 0.5;
        
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
        
        result += (diffuse + specular) * lights[i].intensity * attenuation;
    }
    
    FragColor = vec4(result, 1.0);
}
