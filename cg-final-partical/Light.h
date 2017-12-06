#pragma once

#include <GL/GLUT.H>
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
class Light
{
public:
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation = glm::vec3(1, 0, 0))
	{
		this->Position = position;
		this->Color = color;
		this->Attenuation = attenuation;
	}

	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Attenuation;
};

