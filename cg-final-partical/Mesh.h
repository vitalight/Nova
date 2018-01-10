#pragma once

#include <GL/glew.h>
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "Light.h"
using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

/***************************************************************
* [Mesh]
*
*	This is basically the same with the tutorial online.
*	[Mesh] is the submodule of [Model]. They contains the 
*	vertices, indices, textures and colors of a 3D item. 
*	A [Model] may contain multiple [Mesh].
****************************************************************/
class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	vector<glm::vec3> colors;
	Material material;

	unsigned int VAO, VBO, EBO;

	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, glm::vec3 color);
	Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices);

	// Set shader uniform, bind texture, then render
	void Draw(Light light, Camera camera, Shader shader);

private:
	// setup VAO, VBO and EBO
	void setupMesh();
};