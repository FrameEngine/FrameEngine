#version 450 core

#define MAX_LIGHTS 10

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform PointLight lights[MAX_LIGHTS];
uniform int numLights;
uniform vec3 objectColor;

void main() {
    vec3 result = vec3(0.0);

    vec3 norm = normalize(Normal);

    for (int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color;

        vec3 viewDir = normalize(-FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = 0.5 * spec * lights[i].color;

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

        result += (diffuse + specular) * lights[i].intensity * attenuation;
    }

    FragColor = vec4(result * objectColor, 1.0);
}
