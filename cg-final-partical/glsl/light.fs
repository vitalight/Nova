#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;
in vec3 pass_fragPos;
in vec3 pass_normal;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;

void main(void){
	// rim lighting
	vec3 viewDir = normalize(viewPos - pass_fragPos);
	float intensity = 1.0 - max(dot(viewDir, normalize(pass_normal)), 0.0);
	float rim = smoothstep(0.2, 1.0, intensity);

	out_color = vec4(vec3(texture(texture_diffuse1, pass_texCoord))*0.9 + vec3(1, 0.2, 0.2)*rim, 1.0);
}