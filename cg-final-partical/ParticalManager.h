#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"
#include "Config.h"

class ParticalFlying
{
public:
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 axis;
	GLfloat scale, angle;
};

class ParticalCircling
{
public:
	GLfloat angle;
	GLfloat radius;
	GLfloat y;
	GLfloat scale;
	GLfloat rotAngle;
};

class ParticalFire
{
public:
	GLfloat elapseTime = 0;
	glm::vec3 velocity;
	glm::vec3 position;
};

class ParticalManager
{
public:
	ParticalManager(string name, int _amountFlying, int _amountCircling, float radius, float offset);
	void draw(Light & light, Camera & camera, float & time);
	void switchPartical();
private:
	unsigned int rockVBO, fireVBO;
	Model *rock, *fire;
	int amountFlying, amountCircling, amountFireMax;
	float liveRange = 0.8;
	std::vector<ParticalFlying> flyingRocks;
	ParticalCircling *circlingRocks;
	std::vector<ParticalFire> particalFires;

	glm::mat4 *rockMatrices;
	glm::mat4 *fireMatrices;

	void update(const float time, Camera &camera);
	void generateFire(Camera & camera);
	bool checkLiveness(ParticalFlying & particalFlying);
	void generateCirclingPartical(const float radius, const float offset);
	void generatePartical();
	void setupModel(Model * rock);
	void drawModel(Model * model, int number, Light & light, Camera & camera, float & time);
};

