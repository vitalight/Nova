#include "ResourceManager.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, Model*> ResourceManager::Models;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::LoadShader(const GLchar * shaderName)
{
	GLchar vShaderFile[32],
		   fShaderFile[32];
	sprintf_s(vShaderFile, "glsl/%s.vs", shaderName);
	sprintf_s(fShaderFile, "glsl/%s.fs", shaderName);
	Shaders[shaderName] = loadShaderFromFile(vShaderFile, fShaderFile, nullptr);
	return Shaders[shaderName];
}

Shader ResourceManager::GetShader(std::string name)
{
	if (Shaders.find(name) == Shaders.end()) {
		std::cout << "Error::ResourceManager::GetShader: No shader named '" << name << "'" << std::endl;
		exit(-1);
	}
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	if (Textures.find(name) == Textures.end()) {
		std::cout << "Error::ResourceManager::GetTexture: No texture named '" << name << "'" << std::endl;
		exit(-1);
	}
	return Textures[name];
}

Model *ResourceManager::LoadModel(std::string path, std::string name, std::string shaderName, glm::vec3 offset, bool gamma)
{
	cout << "Loading from [" << path << "]..." << endl;
	Models[name] = new Model(Shaders[shaderName], path, offset, gamma);
	return Models[name];
}

Model *ResourceManager::LoadModel(std::string name, std::string shaderName, glm::vec3 offset)
{

	cout << "Loading from [" << name << "]..." << endl;
	Models[name] = new Model(Shaders[shaderName], "resources/objects/" + name + "/" + name + ".obj", offset, false);
	return Models[name];
}

Model * ResourceManager::LoadPlanetModel(std::string name, std::string texturePath, std::string shaderName, glm::vec3 offset)
{

	cout << "Loading from [" << name << "]..." << endl;
	if (Models["earth"] == nullptr) {
		LoadModel("earth", "texture", glm::vec3(0));
	}
	Models[name] = new Model(*Models["earth"]);
	Texture texture;
	Texture2D t2d = LoadTexture(texturePath.c_str(), false, name);
	texture.id = t2d.ID;
	texture.type = "texture_diffuse";
	texture.path = texturePath.c_str();
	Models[name]->textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	Models[name]->meshes[0].textures[0] = texture;
	Models[name]->shader = GetShader(shaderName);
	return Models[name];
}

Model *ResourceManager::GetModel(std::string name)
{
	if (Models.find(name) == Models.end()) {
		std::cout << "Error::ResourceManager::GetTexture: No model named '" << name << "'" << std::endl;
		exit(-1);
	}
	return Models[name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);

		if (!vertexShaderFile) {
			std::cout << "Error::ResourceManager::loadShaderFromFile: file '" << vShaderFile << "' don't exist." << std::endl;
			exit(-1);
		}

		if (!fragmentShaderFile) {
			std::cout << "Error::ResourceManager::loadShaderFromFile: file '" << fShaderFile << "' don't exist." << std::endl;
			exit(-1);
		}

		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	/*// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	*/

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *image = stbi_load(file, &width, &height, &nrComponents, 0);
	if (image)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		texture.Generate(width, height, image);
		stbi_image_free(image);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << image << std::endl;
		stbi_image_free(image);
	}

	return texture;
}