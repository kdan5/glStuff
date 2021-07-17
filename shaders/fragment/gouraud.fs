#version 330 core
out vec4 fragColor;

in vec3 lightingColor;

uniform vec3 objColor;

void main() {
    fragColor = vec4(lightingColor * objColor, 1.0);
}

