#version 330 core
in vec2 texi;
flat in int tempi;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
layout(location = 0) out vec4 color;

void main() {
  switch (tempi) {
    case 0:
      color = texture(tex2, texi);
      break;
    case 1:
      color = texture(tex1, texi);
      break;
    case 2:
      color = texture(tex3, texi);
      break;
    case 3:
      color = texture(tex4, texi);
      break;
    case 4:
      color = texture(tex1, texi);
    default:
      break;
  }

//   float l = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
//   color.a = l;

}
