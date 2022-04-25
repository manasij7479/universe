#version 330 core
in vec2 texi;
flat in int tempi;
uniform sampler2D tex;
layout(location = 0) out vec4 color;
in vec2 sp;

void main() {
  color = texture(tex, texi);
  // 0 blue, 1 white, 2 yellow, 3 orange, 4 red
  switch (tempi) {
    case 0:
      color.xyz = vec3(119.0/225, 182.0/225, 216.0/225);
      break;
    case 1:
      break;
    case 2:
      color.rgb = vec3(1, 1, 0);
      break;
    case 3:
      color.rgb = vec3(1, 0.9, 0);
      break;
    case 4:
      color.rgb = vec3(227.0/225, 93.0/225, 120.0/225);
    default:
      break;
  }

  if (abs(sp.x) < 0.2 || abs(sp.y) < 0.2) {
    float dist = sqrt(sp.x * sp.x + sp.y * sp.y);
    color.a += (10 - dist)/20;
  }
  if (color.a > 0.5) color.a += 0.1; // else color.a -= 0.1;

}
