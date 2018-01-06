#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"
#include "Texture.h"

#define PARTICAL_MAX_NUM 100
#define PARTICAL_GENERATE_CYCLE 0.5
#define PARTICAL_CLEANUP_CYCLE 5

#define PARTICAL_CENTER_RANGE 800
#define PARTICAL_RANGE 1200

#define PARTICAL_START 120.0f
// 10.0f
#define PARTICAL_SPEED 10.0f
#define PARTICAL_ROTATE_SPEED 0.6f

class ParticalStatus
{
public:
	glm::vec3 velocity, position, axis;
	float scale, angle;
};

class ParticalManager
{
public:
	ParticalManager(string name, string shaderName, int _amount = PARTICAL_MAX_NUM);
	~ParticalManager();
	void draw(Camera camera, Light light, float time);
private:
	unsigned int VBO;
	Model *model;
	Shader shader;
	int amount;
	float mytime = 0;
	std::vector<ParticalStatus> status;
	glm::mat4 *modelMatrices;

	void generatePartical(bool justStarted = false);
	void deleteDeadPartical();
	void update(float time);
};

