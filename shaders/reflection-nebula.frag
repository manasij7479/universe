#version 330 core
in vec2 texi;
// in vec3 normi;
flat in int tempi;
flat in vec3 offi;

flat in vec3 lighti0;

uniform sampler2D tex;
layout(location = 0) out vec4 color;


void main() {
  color = texture(tex, texi);
  vec3 lightDir = normalize(lighti0);
//   vec3 lightDir = vec3(1, 0, 0);

//   color *= dot(normalize(lightDir), normalize(texture(norm, texi).xyz));
//   color *= dot(normalize(lightDir), normalize(-offi));

  vec3 norm = normalize(-offi);
//   vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, normalize(lightDir)), 0.0);
  float loc = length(offi);
  float ambient1 = loc/400;
  float ambient2 = 1 / ambient1;
  float intensity  =  (diff * 2 + ambient1 + ambient2/2)/2;


  color.a -= 0.61;
  color.a *= 0.4;
//   0 blue, 1 white, 2 yellow, 3 orange, 4 red
  switch (tempi) {
    case 0:
      color.rgb = vec3(119.0/225, 182.0/225, 216.0/225);
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
  color.rgb *= intensity;
//   color.a = clamp(color.a, 0, 1);
}
