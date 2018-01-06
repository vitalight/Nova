#include "Partical.h"

// todo: change to generate radius and angle, then rotate
Partical::Partical(string modelName, unsigned int _amount, float time, float radius, float offset)
{
	amount = _amount;
	particalModel = ResourceManager::GetModel(modelName);
	shader = ResourceManager::GetShader(modelName);

	modelMatrices = new glm::mat4[amount];
	infos = new ParticalInfo[amount];

	srand(time); // initialize random seed
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		ParticalInfo info;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		info.angle = (float)i / (float)amount * 360.0f;
		info.radius = radius + (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		info.y = ((rand() % (int)(2 * offset * 100)) / 100.0f - offset)*0.4;
		float x = sin(info.angle) * info.radius;
		float z = cos(info.angle) * info.radius;
		float y = info.y;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.25 and 1.25f
		info.scale = (rand() % 20) / 20.0f + 0.25f;
		model = glm::scale(model, glm::vec3(info.scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		info.rotAngle = (rand() % 360);
		model = glm::rotate(model, info.rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
		infos[i] = info;
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);

	for (unsigned int i = 0; i < particalModel->meshes.size(); i++)
	{
		unsigned int VAO = particalModel->meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}


Partical::~Partical()
{
	delete modelMatrices;
	delete infos;
}

void Partical::draw(Camera camera, Light light, float deltaTime)
{
	// draw meteorites
	shader.Use().SetMatrix4("projection", camera.Projection);
	shader.SetMatrix4("view", camera.GetViewMatrix());
	shader.SetInteger("texture_diffuse1", 0);

	shader.SetVector3f("lightColor", light.Color);
	shader.SetVector3f("lightPos", light.Position);
	shader.SetVector2f("lightBias", light.LightBias);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particalModel->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < particalModel->meshes.size(); i++)
	{
		glBindVertexArray(particalModel->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, particalModel->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	update(deltaTime);
}

void Partical::update(float deltaTime)
{
	for (int i = 0; i < amount; i++) {
		glm::mat4 model;
		infos[i].angle += deltaTime * ANGLE_SPEED;
		float angle = infos[i].angle,
			radius = infos[i].radius;

		float x = sin(angle) * radius;
		float z = cos(angle) * radius;
		float y = infos[i].y; 

		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = infos[i].scale;
		model = glm::scale(model, glm::vec3(scale));

		infos[i].rotAngle += deltaTime * ROTATE_SPEED;
		float rotAngle = infos[i].rotAngle;
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);
}