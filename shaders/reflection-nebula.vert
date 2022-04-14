#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texc;
layout(location=3) in vec3 off;
layout(location=4) in float size;
layout(location=5) in int temp;

uniform mat4 mvp;
out vec2 texi;
flat out vec3 offi;
flat out int tempi;

void main() {
  gl_Position = mvp * size * vec4(pos * size + off, 1);
  texi = texc;
  tempi = temp;
  offi = off;
}
