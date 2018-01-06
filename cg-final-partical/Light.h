#pragma once

#include <GL/GLUT.H>
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
class Light
{
public:
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 LightBias;

	Light(glm::vec3 pos, glm::vec3 color, glm::vec2 lightBias)
		:Position(pos), Color(color), LightBias(lightBias)
	{
	}

};

