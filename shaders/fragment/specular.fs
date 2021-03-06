#version 330 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 objColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 normalNormal = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float reflectIntensity = 1.0;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = reflectIntensity * spec * lightColor;

    vec3 result = specular * objColor;
    fragColor = vec4(result, 1.0);
}

