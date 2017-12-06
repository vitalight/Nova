#pragma once

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

#include "Model.h"

// 一个显示在屏幕中的物体,存储空间信息
class Entity
{
public:
	glm::vec3 position, axis, scale;
	float angle;
	Model *model;

	Entity(Model *model, glm::vec3 position, glm::vec3 size, float angle, glm::vec3 axis);
	void Draw();
};

