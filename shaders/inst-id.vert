#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texc;
layout(location=3) in vec3 off;

uniform mat4 mvp;
uniform mat3 nt;

// out vec3 fnormal;
out vec2 texi;

void main() {
  gl_Position = mvp * vec4(pos + off, 1);
//   fnormal = nt * normal;
  texi = texc;
}
