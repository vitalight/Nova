#pragma once

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "ResourceManager.h"

// 一个显示在屏幕中的物体,存储空间信息
class Entity
{
public:
	glm::vec3 position, axis, scale;
	glm::vec3 offset;
	float angle;
	float mytime = 0;

	enum Entity_type{ENTITY_NORMAL, ENTITY_PLANET, ENTITY_MOON} type;
	float radius, angular_velocity, rotate_velocity;
	Entity *parent;

	Model *model;

	Entity(Model *model, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), float angle = 0.0f, glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f))
		: model(model), position(position), scale(scale), angle(angle), axis(axis)
	{
		type = ENTITY_NORMAL;
	}

	Entity(Model *model, glm::vec3 position = glm::vec3(0, 0, 0), float scale = 1.0f, float angle = 0.0f, glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f))
		: model(model), position(position), scale(glm::vec3(scale, scale, scale)), angle(angle), axis(axis)
	{
		type = ENTITY_NORMAL;
	}

	Entity(string model, glm::vec3 position = glm::vec3(0, 0, 0), float scale = 1.0f, float angle = 0.0f, glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f))
		: model(ResourceManager::GetModel(model)), position(position), scale(glm::vec3(scale, scale, scale)), angle(angle), axis(axis)
	{
		type = ENTITY_NORMAL;
	}

	void configPlanet(float rotate_velocity, float radius = 0, float angular_velocity = 0)
	{
		type = ENTITY_PLANET;
		this->rotate_velocity = rotate_velocity;
		this->radius = radius;
		this->angular_velocity = angular_velocity;
	}

	void configMoon(Entity *parent, float rotate_velocity, float radius = 0, float angular_velocity = 0)
	{
		type = ENTITY_MOON;
		this->parent = parent;
		this->rotate_velocity = rotate_velocity;
		this->radius = radius;
		this->angular_velocity = angular_velocity;
	}

	void Draw(Light light, Camera camera, float time)
	{
		update(time);
		model->Draw(light, camera, position, scale, angle, axis);
	}

	void update(float time)
	{
		mytime += time;
		if (type == ENTITY_PLANET) {
			angle += time * rotate_velocity;
			position.x = cos(mytime*angular_velocity) * radius;
			position.z = sin(mytime*angular_velocity) * radius;
		}
		else if (type == ENTITY_MOON) {
			angle += time * rotate_velocity;
			position.x = cos(mytime*angular_velocity) * radius + parent->position.x;
			position.z = sin(mytime*angular_velocity) * radius + parent->position.z;
		}
	}
};

