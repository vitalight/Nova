#include "ParticalManager.h"

#define GENERATE_CYCLE 0.5
#define CLEANUP_CYCLE 5

#define CENTER_RANGE 800
#define FULL_RANGE 1200

#define START_FRAME 120.0f
#define PARTICAL_SPEED 10.0f
#define ROTATE_SPEED 0.6f
#define ANGLE_SPEED 0.02f

void printVec3(glm::vec3 vec)
{
	cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]" << endl;
}

ParticalManager::ParticalManager(string name, string shaderName, int _amountFlying, int _amountCircling, float radius, float offset)
{
	model = ResourceManager::GetModel(name);
	shader = ResourceManager::GetShader(shaderName);
	amountFlying = _amountFlying;
	amountCircling = _amountCircling;
	modelMatrices = new glm::mat4[amountFlying + amountCircling];
	for (int i = 0; i < amountFlying/2; i++)
		generatePartical(true);

	infos = new ParticalInfo[amountCircling];

	generateCirclingPartical(radius, offset);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (amountFlying + amountCircling) * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		unsigned int VAO = model->meshes[i].VAO;
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

ParticalManager::~ParticalManager()
{
}

void ParticalManager::draw(Camera camera, Light light, float time)
{
	mytime += time;

	update(time);

	shader.Use().SetMatrix4("projection", camera.Projection);
	shader.SetMatrix4("view", camera.GetViewMatrix());
	shader.SetInteger("texture_diffuse1", 0);

	shader.SetVector3f("lightColor", light.Color);
	shader.SetVector3f("lightPos", light.Position);
	shader.SetVector3f("lightBias", light.LightBias);
	shader.SetVector3f("viewPos", camera.Position);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		glBindVertexArray(model->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amountFlying+amountCircling);
		glBindVertexArray(0);
	}

	deleteDeadPartical();
}

void ParticalManager::deleteDeadPartical()
{
	if ((int)status.size() < amountFlying && mytime < CLEANUP_CYCLE)
		return;

	for (int i = 0; i < (int)status.size(); i++) {
		if (status[i].position.x > FULL_RANGE || status[i].position.x < -FULL_RANGE ||
			status[i].position.y > FULL_RANGE || status[i].position.y < -FULL_RANGE ||
			status[i].position.z > FULL_RANGE || status[i].position.z < -FULL_RANGE) {
			status.erase(status.begin() + i, status.begin() + i + 1);
			//cout << "[log] delete partical #" << i << endl;
			i--;
		}
	}
}

void ParticalManager::update(float time)
{
	if ((int)status.size() < amountFlying && mytime > GENERATE_CYCLE)
		generatePartical();

	int size = status.size();
	for (int i = 0; i < size; i++) {
		glm::mat4 model;

		model = glm::translate(model, status[i].position);

		float scale = status[i].scale;
		model = glm::scale(model, glm::vec3(scale));

		status[i].angle += time * ROTATE_SPEED;
		float rotAngle = status[i].angle;
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		status[i].position += status[i].velocity * time * PARTICAL_SPEED;

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	for (int i = 0; i < amountCircling; i++) {
		glm::mat4 model;
		infos[i].angle += time * ANGLE_SPEED;
		float angle = infos[i].angle,
			radius = infos[i].radius;

		float x = sin(angle) * radius;
		float z = cos(angle) * radius;
		float y = infos[i].y;

		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = infos[i].scale;
		model = glm::scale(model, glm::vec3(scale));

		infos[i].rotAngle += time * ROTATE_SPEED;
		float rotAngle = infos[i].rotAngle;
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[amountFlying+i] = model;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (amountFlying + amountCircling) * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);
}

void ParticalManager::generatePartical(bool justStarted)
{
	//cout << "[log] generate partical #" << status.size() << endl;
	mytime = 0;

	ParticalStatus part;
	part.axis = glm::vec3(0.6f, 0.4f, 0.8f);
	part.angle = (rand() % 20) / 20.0 * PI;
	part.scale = (rand() % 20) / 2.0f + 2.0f;
	int startDirection = rand() % 6;
	float val_1 = rand() % FULL_RANGE - FULL_RANGE/2.0,
		val_2 = rand() % FULL_RANGE - FULL_RANGE/2.0,
		val_3 = rand() % CENTER_RANGE - CENTER_RANGE/2.0,
		val_4 = rand() % CENTER_RANGE - CENTER_RANGE/2.0;
	switch (startDirection)
	{
	case 0:// up
		part.position = glm::vec3(val_1, FULL_RANGE - 1, val_2);
		part.velocity = glm::normalize(glm::vec3(val_3, 1 - FULL_RANGE, val_4) - part.position);
		break;
	case 1:// down
		part.position = glm::vec3(val_1, 1 - FULL_RANGE, val_2);
		part.velocity = glm::normalize(glm::vec3(val_3, FULL_RANGE - 1, val_4) - part.position);
		break;
	case 2:// left
		part.position = glm::vec3(1 - FULL_RANGE, val_1, val_2);
		part.velocity = glm::normalize(glm::vec3(FULL_RANGE - 1, val_3, val_4) - part.position);
		break;
	case 3:// right
		part.position = glm::vec3(FULL_RANGE - 1, val_1, val_2);
		part.velocity = glm::normalize(glm::vec3(1 - FULL_RANGE, val_3, val_4) - part.position);
		break;
	case 4:// front
		part.position = glm::vec3(val_1, val_2, FULL_RANGE - 1);
		part.velocity = glm::normalize(glm::vec3(val_3, val_4, 1 - FULL_RANGE) - part.position);
		break;
	case 5:// back
		part.position = glm::vec3(val_1, val_2, 1 - FULL_RANGE);
		part.velocity = glm::normalize(glm::vec3(val_3, val_4, FULL_RANGE - 1) - part.position);
		break;
	default:
		cout << "error: generate wrong direction" << endl;
		break;
	}
	part.velocity *= rand() % 4 + 1;
	if (justStarted)
		part.position += part.velocity * START_FRAME;
	status.push_back(part);
}

void ParticalManager::generateCirclingPartical(float radius, float offset)
{
	for (unsigned int i = 0; i < amountCircling; i++)
	{
		glm::mat4 model;
		ParticalInfo info;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		info.angle = (float)i / (float)amountCircling * 360.0f;
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
		modelMatrices[amountFlying + i] = model;
		infos[i] = info;
	}
}
