#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"
#include "Config.h"
#include "Entity.h"

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

class FireMatrix
{
public:
	glm::mat4 matrix;
	glm::vec2 texCoord1;
	glm::vec2 texCoord2;
	GLfloat blend;
};

class ParticalManager
{
public:
	ParticalManager(Entity *shuttle, string name, int _amountFlying, int _amountCircling, float radius, float offset);
	void draw(Light & light, Camera & camera, float & time);
	void switchPartical();
	void generateFire(Camera & camera, glm::vec3 direction, glm::vec3 otherDirection1, glm::vec3 otherDirection2);
private:
	unsigned int rockVBO, fireVBO;
	Model *rock, *fire;
	Entity *shuttle;
	int amountFlying, amountCircling, amountFireMax;
	float liveRange = 1.2;
	std::vector<ParticalFlying> flyingRocks;
	ParticalCircling *circlingRocks;
	std::vector<ParticalFire> particalFires;

	glm::mat4 *rockMatrices;
	FireMatrix *fireMatrices;

	void update(const float time);
	bool checkLiveness(ParticalFlying & particalFlying);
	void generateCirclingPartical(const float radius, const float offset);
	void generatePartical();
	void setupModel(Model * rock, bool isFire = false);
	void drawModel(Model * model, int number, Light & light, Camera & camera, float & time);
};

