#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

#include "ResourceManager.h"
#include "Texture.h"

#define ANGLE_SPEED 0.02
#define ROTATE_SPEED 0.5

#define PARTICAL_ASTEROID 0;
#define PARTICAL_SPACESHIP 1;
#define PARTICAL_PIECES 2;

class ParticalInfo 
{
public:
	GLfloat angle;
	GLfloat radius;
	GLfloat y;
	GLfloat scale;
	GLfloat rotAngle;
};

class Partical
{
public:
	Partical(string modelName, unsigned int amount, float time, float radius = 400.0f, float offset = 15.0f);
	~Partical();
	void draw(Camera camera, Light light, float deltaTime);
private:
	int type = 0;
	unsigned int amount, VBO;
	Shader shader;
	Model *particalModel;

	glm::mat4* modelMatrices;
	ParticalInfo *infos;

	void update(float deltaTime);
};

