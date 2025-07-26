#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

uniform vec3 lightPos;     // Position of the light source in world space
uniform vec3 viewPos;      // Camera/view position in world space
uniform vec3 lightColor;   // Light color (usually white)
uniform float ambientStrength; // e.g., 0.1
uniform float specularStrength; // e.g., 0.5
uniform float shininess;        // e.g., 32.0

void main() {
    // Normalize input normals
    vec3 norm = normalize(Normal);

    // Calculate vectors needed for lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting (Lambert)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting (Phong)
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Sample the object's texture color
    vec3 objectColor = texture(texture_diffuse1, TexCoords).rgb;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
