#pragma once

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/stb_image.h"
#include "includes/assimp/Importer.hpp"
#include "includes/assimp/scene.h"
#include "includes/assimp/postprocess.h"

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

const GLfloat PI = 3.1415926535;

/***************************************************************
* [Model]
*
*	This is mostly the same with the tutorial online. It reads
*	the model from file, and setup [Mesh].
*	However, a few changes have been made to achive:
*	
*	1. auto generated normal
*		Some obj files contains no normal or inadequate normal,
*		thus we have to detect them and calculate normal by 
*		ourselves.
*	2. code input model
*		Originally [Model] can only construct a model from file,
*		I made a few changes to make it able to construct a model
*		by input the vectices and indices that is generated 
*		somewhere else.
****************************************************************/
class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;

	string directory;
	glm::vec3 offset;
	bool gammaCorrection;
	Shader shader;
	float brightness = -1.0;

	Model(Shader shader, string const &path, glm::vec3 offset = glm::vec3(0, 0, 0), bool gamma = false);
	Model(Shader shader, vector<Vertex> &vertices, vector<unsigned int> &indices);

	// Set shader, change modelMatrix, then render all meshes
	// Note that if [isPlanet] is true, the model will be rotate to make it look right 
	void Draw(Light light,
		Camera camera,
		glm::vec3 position,
		glm::vec3 size,
		GLfloat rotate,
		glm::vec3 axis,
		bool isPlanet = false);

	// create a texture from image file
	unsigned int TextureFromFile(const string &filename, bool gamma = false);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
private:
	void loadModel(string const &path);

	// traverse each [aiNode]'s [aiMesh] and it's children node
	void processNode(aiNode *node, const aiScene *scene);
	// translate [aiMesh] into [Mesh]
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	glm::vec3 loadColors(aiMaterial *mat);
};


