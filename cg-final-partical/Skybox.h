#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"

/***********************************************************
* [Skybox]
*	This class is a implementation of cubemaps, and is used 
*	to render the background.
*
*	Note that skybox has to be draw at last, otherwise the 
*	depth detection might lead to glitches.
************************************************************/
class Skybox
{
public:
	Skybox(Shader _shader);
	void draw(Camera camera);
private:
	unsigned int texture;
	Shader shader;
	unsigned int VAO;

	unsigned loadCubemap(std::vector<std::string> faces);
};

