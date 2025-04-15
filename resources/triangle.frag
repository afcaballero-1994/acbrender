#version 460 core
out vec4 FragColor;

layout (location = 3) uniform vec2 iResolution;
in vec2 ftex_coords;
layout (binding = 0) uniform sampler2D Tex1;

in vec3 ourColor;

void main() {

  FragColor = texture(Tex1, ftex_coords);
} 
