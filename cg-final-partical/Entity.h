#pragma once

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"

#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"

// һ����ʾ����Ļ�е�����,�洢�ռ���Ϣ
class Entity
{
public:
	glm::vec3 position, axis, scale;
	float angle;

	Model *model;

	Entity(Model *model, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), float angle = 0.0f, glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f))
		: model(model), position(position), scale(scale), angle(angle), axis(axis)
	{
	}

	void Draw(Light light, Camera camera)
	{
		model->Draw(light, camera, position, scale, angle, axis);
	}
};

