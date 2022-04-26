#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texc;
layout(location=3) in float offx;
layout(location=4) in float offy;
layout(location=5) in float offz;
layout(location=6) in float size;
layout(location=7) in int temp;

uniform mat4 mvp;
out vec2 texi;
flat out int tempi;
out vec2 sp;
void main() {
  vec3 off = vec3(offx, offy, offz);
  gl_Position = mvp * vec4(pos * size + off, 1);
  texi = texc;
  tempi = temp;
  sp = pos.xy;
}
