#pragma once

#include <iostream>
#include <vector>
using namespace std;

class IndicesGenerator
{
public:
	vector<int> generateIndexBuffer(int vertexCount)
	{
		int indexCount = (vertexCount - 1) * (vertexCount - 1) * 6;
		vector<int> indices;
		for (int col = 0; col < vertexCount - 1; col++)
		{
			for (int row = 0; row < vertexCount - 1; row++)
			{
				int topLeft = (row * vertexCount) + col;
				int topRight = topLeft + 1;
				int bottomLeft = ((row + 1) * vertexCount) + col;
				int bottomRight = bottomLeft + 1;
				if (row % 2 == 0) {
					storeQuad1(indices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
				else {
					storeQuad2(indices, topLeft, topRight, bottomLeft, bottomRight, col % 2 == 0);
				}
			}
		}
		return indices;
	}

private:
	void storeQuad1(vector<int> &indices, int topLeft, int topRight, int bottomLeft, int bottomRight, bool mixed)
	{
		indices.push_back(topLeft);
		indices.push_back(bottomLeft);
		indices.push_back(mixed ? topRight : bottomRight);
		indices.push_back(bottomRight);
		indices.push_back(topRight);
		indices.push_back(mixed ? bottomLeft : topLeft);
	}

	void storeQuad2(vector<int> &indices, int topLeft, int topRight, int bottomLeft, int bottomRight, bool mixed)
	{
		indices.push_back(topRight);
		indices.push_back(topLeft);
		indices.push_back(mixed ? bottomRight : bottomLeft);
		indices.push_back(bottomLeft);
		indices.push_back(bottomRight);
		indices.push_back(mixed ? topLeft : topRight);
	}
};