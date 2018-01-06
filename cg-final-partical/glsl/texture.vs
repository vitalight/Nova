#version 330 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

out vec2 pass_texCoord;
out vec3 pass_fragPos;
out vec3 pass_normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void){
	gl_Position = projection * view * model * vec4(in_position, 1.0);

	pass_texCoord = in_texCoord;
	pass_fragPos = vec3(model * vec4(in_position, 1.0));
	pass_normal = mat3(transpose(inverse(model))) * in_normal;
}