#version 330 core
#define GLSLIFY 1

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main() {
    vec3 Incident = normalize(o_positionWorld - viewPos);
    vec3 Reflechi = reflect(Incident, normalize(o_normalWorld));
    FragColor = vec4(texture(skybox, Reflechi).rgb, 1.0);
}
