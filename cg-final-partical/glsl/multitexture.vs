#version 330 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoord;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void){
	gl_Position = projection * view * model * vec4(in_position, 1.0);

	// new
	vs_out.FragPos = vec3(model * vec4(in_position, 1.0));   
    vs_out.TexCoords = in_texCoord;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * in_tangent);
    vec3 B = normalize(normalMatrix * in_bitangent);
    vec3 N = normalize(normalMatrix * in_normal);    
    
    mat3 TBN = transpose(mat3(T, B, N));  
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
}