#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in mat4 in_instanceMatrix;

out vec2 pass_texCoord;
out vec3 pass_normal;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	//pass_normal = mat3(transpose(inverse(in_instanceMatrix))) * in_normal;
	//pass_fragPos = vec3(in_instanceMatrix * vec4(in_position, 1.0));
    //gl_Position = projection * view * in_instanceMatrix * vec4(in_position, 1.0f); 
	
	// billboard technique
	mat4 modview = mat4(1.f);
	modview[3] = mat4(view*in_instanceMatrix)[3];
    gl_Position = projection * modview * vec4(in_position, 1.0f);
	pass_texCoord = vec2(in_position) + vec2(0.5, 0.5);
	pass_texCoord.y = 1.0 - pass_texCoord.y;
	pass_texCoord = pass_texCoord / 6.0;
}