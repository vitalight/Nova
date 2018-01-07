#include "EntityManager.h"

EntityManager::EntityManager()
{
}

Entity * EntityManager::createEntity(string name, GLfloat scale)
{
	Entity *entity = new Entity(name, glm::vec3(0), scale);
	entities.push_back(entity);
	return entity;
}

void EntityManager::draw(Light &light, Camera &camera, float time)
{
	for (auto entity : entities)
		entity->Draw(light, camera, time);
}
