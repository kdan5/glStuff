#version 330 core
out vec4 fragColor;

in vec3 fragPos;
in vec3 normal;
in vec3 lightView;

uniform vec3 objColor;
uniform vec3 lightColor;

void main() {
    float ambientIntensity = 0.2;
    vec3 ambient = ambientIntensity * lightColor;

    vec3 normalNormal = normalize(normal);
    vec3 lightDir = normalize(lightView - fragPos);
    float diff = max(dot(normalNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float reflectIntensity = 1.0;
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = reflectIntensity * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objColor;
    fragColor = vec4(result, 1.0);
}

