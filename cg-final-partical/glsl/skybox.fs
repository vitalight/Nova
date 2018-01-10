#version 330 core
out vec4 out_fragColor;

in vec3 pass_texCoord;

uniform samplerCube skybox;

void main()
{    
	out_fragColor = texture(skybox, pass_texCoord);
}