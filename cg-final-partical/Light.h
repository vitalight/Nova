#pragma once

#include <GL/GLUT.H>
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
class Light
{
public:
	glm::vec3 Direction;
	glm::vec3 Color;
	glm::vec2 LightBias;

	Light(glm::vec3 direction, glm::vec3 color, glm::vec2 lightBias)
		:Direction(direction), Color(color), LightBias(lightBias)
	{
	}

};

