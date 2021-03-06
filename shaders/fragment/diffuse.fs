#version 330 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    vec3 normalNormal = normalize(normal);
    vec3 direction = normalize(lightPos - fragPos);
    float diff = max(dot(normalNormal, direction), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = diffuse * objColor;
    fragColor = vec4(result, 1.0);
}

