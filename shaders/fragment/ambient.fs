#version 330 core
out vec4 fragColor;

uniform vec3 objColor;
uniform vec3 lightColor;

void main() {
    float strength = 0.1;
    vec3 ambient= strength * lightColor;
    vec3 result = ambient * objColor;
    fragColor = vec4(result, 1.0);
}
