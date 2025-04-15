#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec3 vtex_coords;

out vec3 ourColor;
out vec2 ftex_coords;

void main()
{
    gl_Position = vec4(pos, 1.0);
    ftex_coords = vtex_coords.xy;

    ourColor = col;
}
