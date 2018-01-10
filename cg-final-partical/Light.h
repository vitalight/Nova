#pragma once

#include <GL/GLUT.H>
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

/***********************************************************
* [Light]
*	This class is just a simple container for 
*   light-related information.
************************************************************/
class Light
{
public:
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 LightBias;

	Light(glm::vec3 pos, glm::vec3 color, glm::vec3 lightBias)
		:Position(pos), Color(color), LightBias(lightBias)
	{
	}

};

