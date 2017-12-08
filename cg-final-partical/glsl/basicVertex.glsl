#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;

flat out vec3 pass_color;//The "flat" qualifier stops the color from being interpolated over the triangles.

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec2 lightBias;
uniform vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

//simple diffuse lighting
vec3 calculateLighting(){
	float brightness = max(normalize(dot(-lightDirection, in_normal)), 0);
	return (lightColor * lightBias.x) + (brightness * lightColor * lightBias.y);
}

void main(void){
	gl_Position = projection * view * model * vec4(in_position, 1.0);
	
	vec3 lighting = calculateLighting();
	pass_color = color * lighting;

    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
}