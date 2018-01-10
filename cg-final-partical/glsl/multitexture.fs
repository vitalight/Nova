#version 330 core
out vec4 out_color;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightBias;
uniform vec3 viewPos;

//simple diffuse lighting
vec3 calculateLighting(){
	vec3 norm = texture(texture_normal1, fs_in.TexCoords).rgb;
	vec3 specularColor = texture(texture_specular1, fs_in.TexCoords).rgb;
	vec3 texColor = texture(texture_diffuse1, fs_in.TexCoords).rgb;

	norm = normalize(norm*2.0 - 1.0);
	// diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diffuse = max(dot(norm, lightDir), 0.0);
	// specular
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float specular = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	// rim lighting
	float intensity = 1.0 - max(dot(normalize(viewDir), norm), 0.0);
	float rim = smoothstep(0.2, 1.0, intensity);
	vec3 rimColor = vec3(0.2, 0.6, 1.5) * rim;
	
	if (diffuse < 0.2) {
		texColor += (0.2-diffuse) * 15 * texture(texture_height1, fs_in.TexCoords).rgb;
	}
	return lightColor * ((lightBias.x + diffuse * lightBias.y)*texColor + specular*lightBias.z*specularColor) * (1 + 1.2*rim) + 0.2*vec3(rim);
}

void main(void){
	vec3 lighting = calculateLighting();

	out_color = vec4(lighting, 1.0);
}