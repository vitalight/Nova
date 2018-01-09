#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;
in vec3 pass_fragPos;
in vec2 pass_texCoord1;
in vec2 pass_texCoord2;
in float pass_blend;

uniform sampler2D texture_diffuse1;

void main(void) {
	vec4 color1 = texture(texture_diffuse1, pass_texCoord1);
	vec4 color2 = texture(texture_diffuse1, pass_texCoord2);
	out_color = mix(color1, color2, pass_blend);
}