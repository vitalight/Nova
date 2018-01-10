#version 330 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texcoord;
layout (location = 3) in mat4 in_instanceMatrix;
layout (location = 7) in vec2 in_texCoord1;
layout (location = 8) in vec2 in_texCoord2;
layout (location = 9) in float in_blend;

out vec2 pass_texCoord;
out vec3 pass_normal;
out vec2 pass_texCoord1;
out vec2 pass_texCoord2;
out float pass_blend;

uniform mat4 projection;
uniform mat4 view;

void main()
{	
	// billboard technique
	mat4 modview = mat4(1.f);
	modview[3] = mat4(view*in_instanceMatrix)[3];

	pass_texCoord = vec2(in_position) + vec2(0.5, 0.5);
	pass_texCoord.y = 1.0 - pass_texCoord.y;
	pass_texCoord = pass_texCoord / 4.0;
	
	pass_texCoord1 = in_texCoord1 + pass_texCoord;
	pass_texCoord2 = in_texCoord2 + pass_texCoord;
	pass_blend = in_blend;
	
    gl_Position = projection * modview * vec4(in_position, 1.0f);
}