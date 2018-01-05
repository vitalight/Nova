#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"
#include "Texture.h"

class Skybox
{
public:
	Skybox(Shader shader);
	~Skybox();
	void init();
	void draw(Camera camera);
private:
	unsigned int texture;
	Shader shader;
	unsigned int VAO;

	unsigned loadCubemap(std::vector<std::string> faces);
};

