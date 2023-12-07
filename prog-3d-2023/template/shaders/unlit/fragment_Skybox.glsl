#version 330 core
#define GLSLIFY 1

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCoords);
}
