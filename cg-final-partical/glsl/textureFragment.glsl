#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;

uniform sampler2D texture_diffuse1;

void main(void){
	out_color = texture(texture_diffuse1, pass_texCoord);
}