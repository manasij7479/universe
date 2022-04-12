#version 330 core
in vec2 texi;
uniform sampler2D tex;
layout(location = 0) out vec4 color;

void main() {
  color = texture(tex, texi);
}
