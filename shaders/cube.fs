#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3      objectColor;
uniform vec3      lightColor;
uniform vec3      lightPos;
uniform bool      useTexture;
uniform sampler2D ourTexture;

const float ambientStrength = 0.1;

void main()
{
    vec3 surface = objectColor;
    if (useTexture)
        surface *= texture(ourTexture, TexCoord).rgb;

    vec3 ambient  = ambientStrength * lightColor;
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse  = max(dot(norm, lightDir), 0.0) * lightColor;

    FragColor = vec4((ambient + diffuse) * surface, 1.0);
}
