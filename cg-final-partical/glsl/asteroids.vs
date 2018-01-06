#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in mat4 in_instanceMatrix;

out vec2 pass_texCoord;
out vec3 pass_fragPos;
out vec3 pass_normal;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    pass_texCoord = in_texcoord;
	pass_normal = mat3(transpose(inverse(in_instanceMatrix))) * in_normal;
	pass_fragPos = vec3(in_instanceMatrix * vec4(in_position, 1.0));
    gl_Position = projection * view * in_instanceMatrix * vec4(in_position, 1.0f); 
}