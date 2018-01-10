#version 330 core
out vec4 out_color;

in vec2 pass_texCoord;
in vec3 pass_fragPos;
in vec3 pass_normal;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightBias;
uniform vec3 viewPos;

vec3 calculateLighting(){
	vec3 norm = normalize(pass_normal);
    vec3 lightDir = normalize(lightPos - pass_fragPos);
	float diffuse = max(dot(norm, lightDir), 0);

	vec3 viewDir = normalize(viewPos - pass_fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	float intensity = 1.0 - max(dot(viewDir, normalize(pass_normal)), 0.0);
	float rim = smoothstep(0.2, 1.0, intensity);

	return lightColor * (lightBias.x + diffuse * lightBias.y + specular * lightBias.z) * (1 + rim) + vec3(0.3*rim);
}

void main(void){
	vec3 lighting = calculateLighting();

	out_color = vec4(vec3(texture(texture_diffuse1, pass_texCoord)) * lighting, 1.0);
}