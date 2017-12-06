#include "Entity.h"

Entity::Entity(Model *model, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
	: model(model), position(position), scale(scale), angle(angle), axis(axis)
{
}

void Entity::Draw()
{
	model->Draw(position, scale, angle, axis);
}