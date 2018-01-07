#pragma once

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

#include "Model.h"
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
	std::vector<glm::mat4> particals;

	enum Entity_type{ENTITY_NORMAL, ENTITY_PLANET, ENTITY_MOON, ENTITY_SHUTTLE} type;
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

	void configShuttle()
	{
		type = ENTITY_SHUTTLE;
	}

	void Draw(Light light, Camera camera, float time)
	{
		update(time);

		switch (type) {
		case ENTITY_SHUTTLE:
			// camera.Position + glm::vec3(0, 0, 10.0f)
			model->Draw(light, camera, camera.Position + camera.Front*40.0f - camera.Up*4.0f, 
				scale, -glm::radians(camera.Yaw)+PI, glm::vec3(0, 1, 0));
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

private:
	void update(float time)
	{
		mytime += time;
		switch (type) {
		case ENTITY_PLANET:
			angle += time * rotate_velocity;
			position.x = cos(-mytime*angular_velocity) * radius;
			position.z = sin(-mytime*angular_velocity) * radius;
			break;
		case ENTITY_MOON:
			angle += time * rotate_velocity;
			position.x = cos(-mytime*angular_velocity) * radius + parent->position.x;
			position.z = sin(-mytime*angular_velocity) * radius + parent->position.z;
			break;
		}
	}
};

