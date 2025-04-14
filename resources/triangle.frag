#version 460 core
out vec4 FragColor;

layout (location = 2) uniform vec2 iResolution;

in vec3 ourColor;

void main() {
  vec2 uvs = gl_FragCoord.xy / iResolution;
  vec3 color = vec3(uvs.x);
  FragColor = vec4(ourColor, 1.0f);
} 
