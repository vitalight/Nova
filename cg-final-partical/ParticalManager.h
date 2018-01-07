#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"
#include "Config.h"

class ParticalStatus
{
public:
	glm::vec3 velocity, position, axis;
	float scale, angle;
};

class ParticalInfo
{
public:
	GLfloat angle;
	GLfloat radius;
	GLfloat y;
	GLfloat scale;
	GLfloat rotAngle;
};

class ParticalManager
{
public:
	ParticalManager(string name, string shaderName, int _amountFlying, int _amountCircling, float radius, float offset);
	void draw(Light & light, Camera & camera, float & time);
	void generateCirclingPartical(const float radius, const float offset);
private:
	unsigned int VBO;
	Model *model;
	Shader shader;
	int amountFlying, amountCircling;
	float mytime = 0;
	std::vector<ParticalStatus> status;
	ParticalInfo *infos;
	glm::mat4 *modelMatrices;

	void generatePartical(const bool justStarted = false);
	void deleteDeadPartical();
	void update(const float time);
};

