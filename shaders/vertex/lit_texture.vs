#version 330 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexCoords;

out vec3 fragPos;
out vec3 normal;
out vec3 lightView;
out vec2 texCoords;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    fragPos = vec3(view * model * vec4(vPos, 1.0));
    normal = mat3(transpose(inverse(view * model))) * vNormal;
    lightView = vec3(view * vec4(lightPos, 1.0));
    texCoords = vTexCoords;
}
