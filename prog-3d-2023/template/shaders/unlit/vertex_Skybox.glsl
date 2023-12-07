#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main() {
  TexCoords = position;
  vec3 scale = position * 1000.0;
  vec4 positionWorld = model * vec4(scale, 1.0);
  gl_Position = projection * view * positionWorld;
}

