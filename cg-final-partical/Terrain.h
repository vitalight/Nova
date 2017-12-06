#pragma once

#include "TerrainRenderer.h"

class Terrain
{
public:
	unsigned int VAO;
	int vertexCount;
	TerrainRenderer *renderer;

	Terrain(unsigned int VAO, int vertexCount, TerrainRenderer *renderer);
};

