#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;
in vec3 pass_fragPos;

uniform sampler2D texture_diffuse1;

void main(void){
	out_color = vec4(vec3(texture(texture_diffuse1, pass_texCoord)), 1.0);
	//if (out_color.rgb == vec3(0, 0, 0))
	//	discard;
}