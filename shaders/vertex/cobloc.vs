#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 pos = aPos;

uniform float xOffset;
uniform float yOffset;

void main() {
    gl_Position = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0f);
}
