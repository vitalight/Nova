#version 330 core
layout (location = 0) in vec3 in_position;

out vec3 pass_texCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    pass_texCoord = in_position;
    vec4 pos = projection * view * vec4(in_position, 1.0);
    gl_Position = pos.xyww;
}  