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

class Model
{
public:
	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	glm::vec3 offset;
	bool gammaCorrection;
	Shader shader;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	Model(Shader shader, string const &path, glm::vec3 offset = glm::vec3(0, 0, 0), bool gamma = false);

	Model(Shader shader, vector<Vertex> &vertices, vector<glm::vec3> &colors, vector<unsigned int> &indices);

	// draws the model, and thus all its meshes
	void Draw(Light light,
		Camera camera,
		glm::vec3 position,
		glm::vec3 size,
		GLfloat rotate,
		glm::vec3 axis,
		bool isPlanet = false);

	unsigned int TextureFromFile(const string &filename, bool gamma = false);

	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
private:
	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string const &path);

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	glm::vec3 loadColors(aiMaterial *mat);
};


