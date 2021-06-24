#version 330 core
out vec4 fragColor;

in vec3 color;
in vec2 texCoords;

uniform sampler2D fTexture0;
uniform sampler2D fTexture1;

void main() {
    fragColor = mix(texture(fTexture0, texCoords), texture(fTexture1, vec2(1.0 - texCoords.x, texCoords.y)), 0.2);
}
