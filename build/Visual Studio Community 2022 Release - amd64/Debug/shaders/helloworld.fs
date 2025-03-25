
#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main() {
  vec2 st = gl_FragCoord.xy / u_resolution.xy;
  vec3 color = vec3(0.0);

  vec2 pos = vec2(0.5) - st;

  float r = length(pos) * 2.0;
  float a = atan(pos.y, pos.x);

  float f = 0.0;

  //   f = cos(a * 3.0 + u_time * 20.0);
//   f = abs(cos((a * 3.0 + 20.0 * u_time) * u_time));
  f = abs(cos((a * 3.0 )));
  //   f = abs(cos(a * 2.5)) * .5 + .3;
  // f = abs(cos(a * 12.) * sin((a * 3. + u_time) * u_time)) * .8 + .1;
  //   f = smoothstep(-.5, 1., cos(a * 10. * u_time)) * 0.2 + 0.5;

  color = vec3(1.0 - smoothstep(f, f + 0.02, r));

  gl_FragColor = vec4(color, 1.0);
}
