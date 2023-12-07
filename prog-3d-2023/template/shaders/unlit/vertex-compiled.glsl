#version 330 core
#define GLSLIFY 1

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos; 

out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main() {
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  o_uv0 = uv0;
  vec4 positionWorld = model * vec4(position, 1.0);
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;

  vec3 Pos = vec3(model * vec4(position, 1.0));

  vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
  vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
    
  mat3 TBN = transpose(mat3(T, B, N));    
  TangentLightPos = TBN * lightPos;
  TangentViewPos  = TBN * viewPos;
  TangentFragPos  = TBN * Pos;

  gl_Position = projection * view * positionWorld;
}
