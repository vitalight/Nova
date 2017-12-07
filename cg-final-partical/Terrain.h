#pragma once

#include "TerrainRenderer.h"

class Terrain
{
public:
	unsigned int VAO;
	int vertexCount;
	TerrainRenderer *renderer;

	Terrain(unsigned int VAO, int vertexCount, TerrainRenderer *renderer);

	void render(Camera *camera, Light *light)
	{
		renderer->render(this, camera, light);
	}
};

