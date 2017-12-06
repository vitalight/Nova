#pragma once

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

#include "Model.h"

// һ����ʾ����Ļ�е�����,�洢�ռ���Ϣ
class Entity
{
public:
	glm::vec3 position, axis, scale;
	float angle;
	Model *model;

	Entity(Model *model, glm::vec3 position, glm::vec3 size, float angle, glm::vec3 axis);
	void Draw();
};

