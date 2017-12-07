#include "TerrainRenderer.h"



TerrainRenderer::TerrainRenderer(Shader *shader, bool hasIndices)
	: shader(shader), hasIndices(hasIndices)
{
}

void TerrainRenderer::render(Terrain *terrain, Camera *camera, Light *light)
{
	prepare(terrain, camera, light);
	if (hasIndices) {
		glDrawElements(GL_TRIANGLES, terrain->vertexCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, terrain->vertexCount);
	}
	finish(terrain);
}

void TerrainRenderer::prepare(Terrain *terrain, Camera *camera, Light *light)
{
	glBindVertexArray(terrain->VAO);
	shader->Use();
	shader->SetVector2f("lightBias", light->LightBias);
	shader->SetVector3f("lightDirection", light->Direction);
	shader->SetVector3f("lightColor", light->Color);
	shader->SetMatrix4("projectionViewMatrix", camera->GetViewMatrix());
}

void TerrainRenderer::finish(Terrain *terrain)
{
	glBindVertexArray(0);
	glUseProgram(0);
}