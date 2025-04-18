#version 460 core
out vec4 FragColor;

layout (location = 3) uniform vec2 iResolution;
in vec2 ftex_coords;
layout (binding = 0) uniform sampler2D Tex1;

in vec3 ourColor;

vec2 uv = gl_FragCoord.xy / iResolution;

void main() {
  const vec3 W = vec3(0.2125f, 0.7154, 0.0721);
  vec3 col = vec3(uv.x);
  //FragColor = vec4(col, 1.0f);
  vec4 newcol = vec4(col, 1.0f);

  float luminance = dot(texture(Tex1, ftex_coords).rgb, W);

  FragColor = vec4(vec3(luminance), 1.0f) * newcol;
} 
