#version 330 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vNormal;

out vec3 lightingColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    // gouraud shading
    vec3 position = vec3(model * vec4(vPos, 1.0));
    vec3 normal = mat3(transpose(inverse(model))) * vNormal;

    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 normalNormal = normalize(normal);
    vec3 lightDir = normalize(lightPos - position);
    float diff = max(dot(normalNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float reflectIntensity = 0.5;
    vec3 viewDir = normalize(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, normalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    lightingColor = ambient + diffuse + specular;
}
