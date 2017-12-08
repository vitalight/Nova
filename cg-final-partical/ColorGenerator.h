#pragma once
/*
#include <iostream>
#include "includes/glm/glm.hpp"
#include <vector>
using namespace std;

class ColorGenerator
{
public:
	float spread;
	float halfSpread;

	vector<glm::vec3> biomeColors;
	float part;

	ColorGenerator(vector<glm::vec3> biomeColors, float spread)
		: biomeColors(biomeColors), spread(spread), halfSpread(spread/2.0f)
	{
		part = 1.0f / (biomeColors.size() - 1);
	}

	vector<vector<glm::vec3>> generateColors(vector<vector<float>> heights, float amplitude)
	{
		vector<vector<glm::vec3>> colors;
		for (int z = 0; z < heights.size; z++)
		{
			vector<glm::vec3> line;
			for (int x = 0; x < heights[z].size(); x++)
			{
				line.push_back(calculateColor(heights[z][x], amplitude));
			}
			colors.push_back(line);
		}
	}

	glm::vec3 calculateColor(float height, float amplitude)
	{
		float value = (height + amplitude) / (amplitude * 2);
		value = clamp((value - halfSpread) * (1.0f / spread), 0.0f, 0.9999f);
		int firstBiome = (int)floor(value / part);
		float blend = (value - (firstBiome * part)) / part;
		return interpolateColors(biomeColors[firstBiome], biomeColors[firstBiome + 1], blend);

	}

	float clamp(float value, float floor, float top)
	{
		if (value > top) {
			return top;
		}
		if (value < floor) {
			return floor;
		}
		else return value;
	}

	glm::vec3 interpolateColors(glm::vec3 color1, glm::vec3 color2, float blend)
	{
		float color1Weight = 1 - blend;
		float r = (color1Weight * color1.x) + (blend * color2.x);
		float g = (color1Weight * color1.y) + (blend * color2.y);
		float b = (color1Weight * color1.z) + (blend * color2.z);
		return glm::vec3(r, g, b);
	}
};*/