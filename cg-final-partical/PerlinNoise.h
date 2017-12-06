#pragma once

#include <iostream>

class PerlinNoise 
{
public:
	int seed;
	float roughness;
	int octaves;
	float amplitude;

	PerlinNoise(int seed, int octaves, float amplitude, float roughness)
		:seed(seed), octaves(octaves), amplitude(amplitude), roughness(roughness)
	{
	}

	PerlinNoise(int octaves, float amplitude, float roughness)
		: octaves(octaves), amplitude(amplitude), roughness(roughness)
	{
		seed = rand() % 1000000000;
	}

	float getPerlinNoise(int x, int y)
	{
		float total = 0;
		float d = pow(2, octaves - 1);
		for (int i = 0; i < octaves; i++)
		{
			float freq = (float)(pow(2, i) / d),
				amp = (float)(pow(roughness, i)*amplitude);
			total += getInterpolatedNoise(x*freq, y*freq)*amp;
		}
		return total;
	}

	float getInterpolatedNoise(int x, int y)
	{
		int intX = (int)x;
		float fracX = x - intX;
		int intY = (int)y;
		float fracY = y - intY;

		float v1 = getSmoothNoise(intX, intY);
		float v2 = getSmoothNoise(intX + 1, intY);
		float v3 = getSmoothNoise(intX, intY + 1);
		float v4 = getSmoothNoise(intX + 1, intY + 1);
		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);
		return interpolate(i1, i2, fracY);
	}

	float getSmoothNoise(int x, int y)
	{
		float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1)
			+ getNoise(x + 1, y + 1)) / 16.0f;
		float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8f;
		float center = getNoise(x, y) / 4.0f;
		return corners + sides + center;
	}

	float getNoise(int x, int y) {
		srand(x * 49632 + y * 325176 + seed);
		return (rand()) * 2.0f - 1.0f;
	}

	float interpolate(float a, float b, float blend)
	{
		double theta = blend * 3.14159265;
		float f = (float)((1.0f - cos(theta)) * 0.5f);
		return a * (1 - f) + b * f;
	}
};