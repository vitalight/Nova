#include "Entity.h"

Entity::Entity(Model *model, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
	: model(model), position(position), scale(scale), angle(angle), axis(axis), type(ENTITY_NORMAL)
{
}

Entity::Entity(Model *model, glm::vec3 position, float scale, float angle, glm::vec3 axis)
	: model(model), position(position), scale(glm::vec3(scale)), angle(angle), axis(axis), type(ENTITY_NORMAL)
{
}

Entity::Entity(string model, glm::vec3 position, float scale, float angle, glm::vec3 axis)
	: model(ResourceManager::GetModel(model)), position(position), scale(glm::vec3(scale)), angle(angle), axis(axis), type(ENTITY_NORMAL)
{
}

void Entity::configPlanet(float rotate_velocity, float radius, float angular_velocity)
{
	type = ENTITY_PLANET;
	this->rotate_velocity = rotate_velocity;
	this->radius = radius;
	this->angular_velocity = angular_velocity;
}

void Entity::configMoon(Entity *parent, float rotate_velocity, float radius, float angular_velocity)
{
	type = ENTITY_MOON;
	this->parent = parent;
	this->rotate_velocity = rotate_velocity;
	this->radius = radius;
	this->angular_velocity = angular_velocity;
}

void Entity::configShuttle()
{
	type = ENTITY_SHUTTLE;
}

void Entity::Draw(Light light, Camera camera, float time)
{
	update(time);

	switch (type) {
	case ENTITY_SHUTTLE:
		model->Draw(light, camera, camera.Position + camera.Front*40.0f - camera.Up*4.0f,
			scale, -glm::radians(camera.Yaw) + PI, glm::vec3(0, 1, 0));
		break;
	case ENTITY_PLANET:
	case ENTITY_MOON:
		model->Draw(light, camera, position, scale, angle, axis, true);
		break;
	default:
		model->Draw(light, camera, position, scale, angle, axis);
		break;
	}
}

void Entity::update(float time)
{
	mytime += time;
	switch (type) {
	case ENTITY_PLANET:
		angle += time * rotate_velocity * NV_ROTATE_WEIGHT;
		position.x = cos(-mytime*angular_velocity) * radius;
		position.z = sin(-mytime*angular_velocity) * radius;
		break;
	case ENTITY_MOON:
		angle += time * rotate_velocity * NV_ROTATE_WEIGHT;
		position.x = cos(-mytime*angular_velocity) * radius + parent->position.x;
		position.z = sin(-mytime*angular_velocity) * radius + parent->position.z;
		break;
	}
}

