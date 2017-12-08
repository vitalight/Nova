#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;

flat out vec3 pass_color;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec2 lightBias;
uniform vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec3 calculateLighting(){
	vec3 normal = normalize(in_normal) * 2.0 - 1.0;
	float brightness = max(dot(lightDirection, normal), 0.0f);
	return min((lightColor * lightBias.x) + (brightness * lightColor * lightBias.y), 2);
}

void main(void){
	gl_Position = projection * view * model * vec4(in_position, 1.0f);
	vec3 lighting = calculateLighting();
	pass_color = vec3(0, 0.4, 0) * lighting;

}