#version 330 core
out vec4 fragColor;

uniform vec3 objColor;
uniform vec3 lightColor;

void main() {
    float strength = 0.1;
    vec3 ambientLight = strength * lightColor;
    vec3 result = ambientLight * objColor;
    fragColor = vec4(result, 1.0);
}
