#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 vtex_coords;

layout (location = 4) uniform mat4 projection;
layout (location = 5) uniform mat4 view;
layout (location = 6) uniform mat4 model;

out vec3 ourColor;
out vec2 ftex_coords;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    //gl_Position = vec4(pos, 1.0);
    ftex_coords = vtex_coords.xy;

    ourColor = col;
}
