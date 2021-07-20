#version 330 core

struct Materials {
    float matShine;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 ambient = lightColor * material.ambient;

    vec3 normalNormal = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normalNormal, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = lightColor * (spec * material.specular);

    vec3 result = (ambient + diffuse + specular) * objColor;
    fragColor = vec4(result, 1.0);
}

