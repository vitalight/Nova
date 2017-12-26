#pragma once

#include <time.h>
#include "includes/glm/glm.hpp"
#include "Camera.h"
#include "Light.h"
#include "Entity.h"
#include "Mesh.h"

#define T_INDEX(X, Y, S) ((Y)*(S+1) + (X))

class Terrain
{
public:
	unsigned int size;
	float tick;
	Shader shader;
	vector<vector<float>> heights;

	Terrain(unsigned int size, float tick, Shader shader)
		:size(size), tick(tick), shader(shader)
	{

	}

	Entity *generateTerrainEntity()
	{
		vector<Vertex> vertices;
		vector<glm::vec3> colors;
		vector<unsigned int> indices;
		prepare(vertices, colors, indices);
		Model *model = new Model(shader, vertices, colors, indices);
		return new Entity(model, glm::vec3(-0.5*size*tick, 0, -0.5*size*tick), 1);
		
	}

	void prepare(vector<Vertex> &vertices, vector<glm::vec3> &colors, vector<unsigned int> &indices)
	{
		// generate vertices and colors
		float height = 0;
		for (unsigned x = 0; x <= size; x++) {
			vector<float> line;
			//cout << endl;
			for (unsigned z = 0; z <= size; z++) {
				height += myrand(0, 2);
				//cout << height << ", ";
				//cout << "height: " << height << endl;

				Vertex vertex;
				vertex.Position = glm::vec3(x*tick, height, z*tick);
				vertex.Normal = glm::vec3(0, 1, 0);
				vertices.push_back(vertex);

				colors.push_back(glm::vec3(0, myrand(0.7, 0.3), 0));
				line.push_back(height);
			}
			heights.push_back(line);
		}

		// generate indices
		for (unsigned x = 0; x < size; x++) {
			for (unsigned z = 0; z < size; z++) {
				indices.push_back(T_INDEX(x, z, size));
				indices.push_back(T_INDEX(x+1, z, size));
				indices.push_back(T_INDEX(x + 1, z+1, size));
				vertices[T_INDEX(x + 1, z + 1, size)].Normal =
					(glm::cross((vertices[T_INDEX(x + 1, z + 1, size)].Position - vertices[T_INDEX(x + 1, z, size)].Position),
						(vertices[T_INDEX(x, z, size)].Position - vertices[T_INDEX(x + 1, z, size)].Position)));
				/*cout << "[debug] normal = " << vertices[T_INDEX(x + 1, z + 1, size)].Normal.x<<", "<<
					vertices[T_INDEX(x + 1, z + 1, size)].Normal.y << ", " <<
					vertices[T_INDEX(x + 1, z + 1, size)].Normal.z << endl;*/
				

				indices.push_back(T_INDEX(x+1, z+1, size));
				indices.push_back(T_INDEX(x, z+1, size));
				indices.push_back(T_INDEX(x, z, size));
			}
		}
	}

	float myrand(float middle, float vary) 
	{
		return ((rand() % 1000) / 1000.0 - 0.5)*vary + middle;
	}
};