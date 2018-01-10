#include "Entity.h"

Entity::Entity(Model *model, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
	: model(model), position(position), scale(scale), angle(angle), axis(axis), type(ENTITY_NORMAL)
{
}

Entity::Entity(Model *model, glm::vec3 position, float scale, float angle, glm::vec3 axis)
	: model(model), position(position), scale(glm::vec3(scale)), angle(angle), axis(axis), type(ENTITY_NORMAL)
{
}

Entity::Entity(string modelName, glm::vec3 position, float scale, float angle, glm::vec3 axis)
	: model(ResourceManager::GetModel(modelName)), position(position), scale(glm::vec3(scale)), angle(angle), axis(axis), type(ENTITY_NORMAL)
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

void Entity::configShuttle(Camera &camera)
{
	type = ENTITY_SHUTTLE;
	position = camera.Position + camera.Front* NV_CAMERA_FRONT_DISTANCE - camera.Up * NV_CAMERA_DOWN_DISTANCE;// -camera.Up*6.0f;
	angle = -glm::radians(camera.Yaw) + PI;
	axis = glm::vec3(0, 1, 0);
}

void Entity::Draw(Light light, Camera camera, float time)
{
	update(time);

	switch (type) {
	case ENTITY_SHUTTLE:
		model->Draw(light, camera, position, scale, angle, axis);
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

void Entity::switchShader()
{
	static bool goodShader = true;
	if (goodShader) {
		model->shader = ResourceManager::GetShader("ugly");
		goodShader = false;
	}
	else {
		model->shader = ResourceManager::GetShader("multitexture");
		goodShader = true;
	}
}

glm::vec3 Entity::getTail()
{
	return position + 2.3f * glm::vec3(cos(angle), 0.1, -sin(angle));
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

