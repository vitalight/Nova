#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;
in vec3 pass_fragPos;
in vec3 pass_normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec2 lightBias;

//simple diffuse lighting
vec3 calculateLighting(){
	vec3 norm = normalize(pass_normal);
    vec3 lightDir = normalize(lightPos - pass_fragPos);

	float brightness = max(dot(norm, lightDir), 0);
	return (lightColor * lightBias.x) + (brightness * lightColor * lightBias.y);
}

void main(void){
	vec3 lighting = calculateLighting();

	out_color = vec4(vec3(texture(texture_diffuse1, pass_texCoord)) * lighting, 1.0);
}