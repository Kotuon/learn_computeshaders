#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;

uniform float t;

void main() {
  vec3 color = vec3(0.0);

  vec2 pos = vec2(0.5) - TexCoords;

  float r = length(pos) * 2.0;
  float a = atan(pos.y, pos.x);

  float f = 0.0;

  f = abs(cos(a * 3.0 + 20.0 * t));

  color = vec3(1.0 - smoothstep(f, f + 0.02, r));

  FragColor = vec4(color, 1.0);
}