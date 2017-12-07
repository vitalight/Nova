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
	TerrainRenderer *renderer;

	TerrainGenerator(PerlinNoise *perlinNoise, ColorGenerator *colorGen, IndicesGenerator *indicesGenerator)
		:perlinNoise(perlinNoise), colorGen(colorGen), indicesGenerator(indicesGenerator)
	{

	}
	
	Terrain *createTerrain(vector<vector<float>> heights, vector<vector<glm::vec3>> colors)
	{
		int vertexCount = calculateVertexCount(heights.size());
		vector<glm::vec3> terrainData = createMeshData(heights, colors, vertexCount);
		// unsigned int ?
		vector<int> indices = indicesGenerator->generateIndexBuffer(heights.size());
		unsigned int VAO, VBO, EBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, terrainData.size() * sizeof(Vertex), &terrainData[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		// todo
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);

		return new Terrain(VAO, indices.size(), renderer);
	}

	void cleanUp();

private:
	int calculateVertexCount(int vertexLength)
	{
		int bottom2Rows = 2 * vertexLength;
		int remainingRowCount = vertexLength - 2;
		int topCount = remainingRowCount * (vertexLength - 1) * 2;
		return topCount + bottom2Rows;
	}

	vector<glm::vec3> createMeshData(vector<vector<float>> heights, vector<vector<glm::vec3>> colors, int vertexCount)
	{
		int byteSize = VERTEX_SIZE_BYTES * vertexCount;
		vector<glm::vec3> lastRow;
		for (int row = 0; row < heights.length - 1; row++) {
			for (int col = 0; col < heights[row].length - 1; col++) {
				GridSquare square = new GridSquare(row, col, heights, colours);
				square.storeSquareData(buffer);
				if (row == heights.length - 2) {
					lastRow.push_back(square);
				}
			}
		}
		for (int i = 0; i < lastRow.length; i++) {
			lastRow[i].storeBottomRowData(buffer);
		}
		return buffer.array();
	}
};

