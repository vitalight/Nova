#pragma once
#include <vector>
#include "Terrain.h"
#include "IndicesGenerator.h"
#include "PerlinNoise.h"
#include "ColorGenerator.h"
class TerrainGenerator
{
public:
	int VERTEX_SIZE_BYTES = 12 + 4 + 4;
	PerlinNoise *perlinNoise;
	ColorGenerator *colorGen;
	IndicesGenerator *indicesGenerator;
	TerrainRenderer renderer;

	TerrainGenerator(PerlinNoise *perlinNoise, ColorGenerator *colorGen, IndicesGenerator *indicesGenerator)
		:perlinNoise(perlinNoise), colorGen(colorGen), indicesGenerator(indicesGenerator)
	{

	}
	
	Terrain createTerrain(vector<vector<float>> heights, vector<vector<glm::vec3>> colors)
	{

	}

	void cleanUp();

private:
	byte[] getMeshData(vector<vector<float>> heights, vector<vector<glm::vec3>> normals, vector<vector<glm::vec3>> colors)
	{
		int byteSize = VERTEX_SIZE_BYTES * heights.
	}
};

