#pragma once

#include "Entity.h"
#include <vector>

class EntityManager
{
public:
	EntityManager();
	Entity * createEntity(string name, GLfloat scale);
	void draw(Light & light, Camera & camera, float time);
private:
	std::vector<Entity *> entities;
};

