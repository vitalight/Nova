#include "ParticalManager.h"

#define USE_FIRE

ParticalManager::ParticalManager(Entity *shuttle, string name, int _amountFlying, int _amountCircling, float radius, float offset)
{
	amountFlying = _amountFlying;
	amountCircling = _amountCircling;
	amountFireMax = 1000;

	// rock partical
	this->shuttle = shuttle;
	rock = ResourceManager::GetModel(name);
	Model *myrobot = ResourceManager::GetModel("myrobot");
	rockMatrices = new glm::mat4[amountFlying + amountCircling];
	for (int i = 0; i < amountFlying; i++)
		generatePartical();

	circlingRocks = new ParticalCircling[amountCircling];
	generateCirclingPartical(radius, offset);

	glGenBuffers(1, &rockVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rockVBO);
	glBufferData(GL_ARRAY_BUFFER, (amountFlying + amountCircling) * sizeof(glm::mat4), &rockMatrices[0], GL_STREAM_DRAW);

	setupModel(rock);
	setupModel(myrobot);

	// fire partical
#ifdef USE_FIRE
	fire = ResourceManager::LoadFireModel("fire", "resources/atlas/particleAtlas.png");
	fireMatrices = new FireMatrix[amountFireMax];
	glGenBuffers(1, &fireVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glBufferData(GL_ARRAY_BUFFER, amountFireMax*sizeof(FireMatrix), &fireMatrices[0], GL_STREAM_DRAW);
	setupModel(fire, true);
#endif
}

void ParticalManager::setupModel(Model *model, bool isFire)
{
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		unsigned int VAO = model->meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		if (isFire) {
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)(3 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)(4 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)(4 * sizeof(glm::vec4) + sizeof(glm::vec2)));
			glEnableVertexAttribArray(9);
			glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(FireMatrix), (void*)(6 * sizeof(glm::vec4) + 2*sizeof(glm::vec2)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glVertexAttribDivisor(9, 1);
		}
		else {
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
		}
		glBindVertexArray(0);
	}
}

void ParticalManager::draw(Light &light, Camera &camera, float &time)
{
	update(time);

	drawModel(rock, amountFlying + amountCircling, light, camera, time);
#ifdef USE_FIRE
	if (particalFires.size()) {
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		drawModel(fire, particalFires.size(), light, camera, time);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}
#endif
}

void ParticalManager::drawModel(Model *model, int number, Light &light, Camera &camera, float &time)
{
	model->shader.Use().SetMatrix4("projection", camera.Projection);
	model->shader.SetMatrix4("view", camera.GetViewMatrix());
	model->shader.SetInteger("texture_diffuse1", 0);

	model->shader.SetVector3f("lightColor", light.Color);
	model->shader.SetVector3f("lightPos", light.Position);
	model->shader.SetVector3f("lightBias", light.LightBias);
	model->shader.SetVector3f("viewPos", camera.Position);
	if (model->textures_loaded.size()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	}
	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		glBindVertexArray(model->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, number);
		glBindVertexArray(0);
	}
}

void ParticalManager::switchPartical()
{
	static bool isRock = true;
	if (isRock) {
		rock = ResourceManager::GetModel("myrobot");
		isRock = false;
	}
	else {
		rock = ResourceManager::GetModel("asteroids");
		isRock = true;
	}
}

bool ParticalManager::checkLiveness(ParticalFlying &particalFlying)
{
	return particalFlying.position.x < NV_FULL_RANGE && particalFlying.position.x > -NV_FULL_RANGE &&
		particalFlying.position.y < NV_FULL_RANGE && particalFlying.position.y > -NV_FULL_RANGE &&
		particalFlying.position.z < NV_FULL_RANGE && particalFlying.position.z > -NV_FULL_RANGE;
}

void ParticalManager::update(const float time)
{
	// update flying rocks
	for (int i = 0; i < amountFlying; i++) {
		glm::mat4 rockMat;
		rockMat = glm::translate(rockMat, flyingRocks[i].position);

		float scale = flyingRocks[i].scale;
		rockMat = glm::scale(rockMat, glm::vec3(scale));

		flyingRocks[i].angle += time * NV_ROTATE_SPEED;
		float rotAngle = flyingRocks[i].angle;
		rockMat = glm::rotate(rockMat, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		if (checkLiveness(flyingRocks[i])) {
			flyingRocks[i].position += flyingRocks[i].velocity * time * NV_PARTICAL_SPEED;
		}
		else {
			flyingRocks[i].velocity = -flyingRocks[i].velocity;
			flyingRocks[i].position += flyingRocks[i].velocity * NV_PARTICAL_SPEED;
		}

		// 4. now add to list of matrices
		rockMatrices[i] = rockMat;
	}

	// update circling rocks
	for (int i = 0; i < amountCircling; i++) {
		glm::mat4 rockMat;
		circlingRocks[i].angle += time * NV_ANGLE_SPEED;
		float angle = circlingRocks[i].angle,
			radius = circlingRocks[i].radius;

		float x = sin(angle) * radius;
		float z = cos(angle) * radius;
		float y = circlingRocks[i].y;

		rockMat = glm::translate(rockMat, glm::vec3(x, y, z));

		float scale = circlingRocks[i].scale;
		rockMat = glm::scale(rockMat, glm::vec3(scale));

		if (circlingRocks[i].rotAngle > 90) {
			circlingRocks[i].rotAngle += time * NV_ROTATE_SPEED;
			float rotAngle = circlingRocks[i].rotAngle;
			rockMat = glm::rotate(rockMat, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		}
		// 4. now add to list of matrices
		rockMatrices[amountFlying+i] = rockMat;
	}

	glBindBuffer(GL_ARRAY_BUFFER, rockVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (amountFlying + amountCircling) * sizeof(glm::mat4), &rockMatrices[0]);
#ifdef USE_FIRE
	// update fire particals
	for (int i = 0; i < particalFires.size(); i++) {
		glm::mat4 fireMat;
		particalFires[i].position = shuttle->getTail() + particalFires[i].elapseTime * particalFires[i].velocity;// time * particalFires[i].velocity;
		particalFires[i].elapseTime += time;
		if (particalFires[i].elapseTime > liveRange) {
			particalFires.erase(particalFires.begin() + i);
			i--;
			continue;
		}
		fireMat = glm::translate(fireMat, glm::vec3(particalFires[i].position));
		fireMatrices[i].matrix = fireMat;

		double stage = particalFires[i].elapseTime / liveRange * 8;
		int stage1 = int(stage);
		int stage2 = stage1 + 1;
		float blend = stage - stage1;
		fireMatrices[i].texCoord1 = glm::vec2(stage1 % 4 * 0.25, stage1 / 4 * 0.25);
		fireMatrices[i].texCoord2 = glm::vec2(stage2 % 4 * 0.25, stage2 / 4 * 0.25);
		fireMatrices[i].blend = blend;
	}
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particalFires.size()*sizeof(FireMatrix), &fireMatrices[0]);
#endif
}

void ParticalManager::generateFire(Camera &camera, glm::vec3 direction, glm::vec3 otherDirection1, glm::vec3 otherDirection2)
{
	if (particalFires.size() >= amountFireMax)
		return;
	ParticalFire part;
	float rand1 = NV_FIRE_RAND, rand2 = NV_FIRE_RAND;
	part.velocity = -NV_FIRE_SPEED * direction + rand1*otherDirection1*0.5f + rand2*otherDirection2*0.5f;
	part.position = shuttle->getTail();
	particalFires.push_back(part);
}

void ParticalManager::generatePartical()
{
	ParticalFlying part;
	part.axis = glm::vec3(0.6f, 0.4f, 0.8f);
	part.angle = (rand() % 20) / 20.0 * PI;
	part.scale = (rand() % 20) / 2.0f + 2.0f;
	int startDirection = rand() % 6;
	float val_1 = rand() % NV_FULL_RANGE - NV_FULL_RANGE / 2.0,
		val_2 = rand() % NV_FULL_RANGE - NV_FULL_RANGE / 2.0,
		val_3 = rand() % NV_CENTER_RANGE - NV_CENTER_RANGE / 2.0,
		val_4 = rand() % NV_CENTER_RANGE - NV_CENTER_RANGE / 2.0;
	switch (startDirection)
	{
	case 0:// up
		part.position = glm::vec3(val_1, NV_FULL_RANGE - 1, val_2);
		part.velocity = glm::normalize(glm::vec3(val_3, 1 - NV_FULL_RANGE, val_4) - part.position);
		break;
	case 1:// down
		part.position = glm::vec3(val_1, 1 - NV_FULL_RANGE, val_2);
		part.velocity = glm::normalize(glm::vec3(val_3, NV_FULL_RANGE - 1, val_4) - part.position);
		break;
	case 2:// left
		part.position = glm::vec3(1 - NV_FULL_RANGE, val_1, val_2);
		part.velocity = glm::normalize(glm::vec3(NV_FULL_RANGE - 1, val_3, val_4) - part.position);
		break;
	case 3:// right
		part.position = glm::vec3(NV_FULL_RANGE - 1, val_1, val_2);
		part.velocity = glm::normalize(glm::vec3(1 - NV_FULL_RANGE, val_3, val_4) - part.position);
		break;
	case 4:// front
		part.position = glm::vec3(val_1, val_2, NV_FULL_RANGE - 1);
		part.velocity = glm::normalize(glm::vec3(val_3, val_4, 1 - NV_FULL_RANGE) - part.position);
		break;
	case 5:// back
		part.position = glm::vec3(val_1, val_2, 1 - NV_FULL_RANGE);
		part.velocity = glm::normalize(glm::vec3(val_3, val_4, NV_FULL_RANGE - 1) - part.position);
		break;
	default:
		cout << "error: generate wrong direction" << endl;
		break;
	}
	part.velocity *= rand() % 5 + 1;
	part.position += part.velocity * NV_INITIAL_FRAME;
	flyingRocks.push_back(part);
}

void ParticalManager::generateCirclingPartical(const float radius, const float offset)
{
	for (int i = 0; i < amountCircling; i++)
	{
		glm::mat4 rock;
		ParticalCircling info;

		info.angle = (float)i / (float)amountCircling * 360.0f;
		info.radius = radius + (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		info.y = ((rand() % (int)(2 * offset * 100)) / 100.0f - offset)*0.4;
		float x = sin(info.angle) * info.radius;
		float z = cos(info.angle) * info.radius;
		float y = info.y;
		rock = glm::translate(rock, glm::vec3(x, y, z));

		info.scale = (rand() % 20) / 5.0f + 0.2f;
		rock = glm::scale(rock, glm::vec3(info.scale));
		
		info.rotAngle = (rand() % 360);
		rock = glm::rotate(rock, info.rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		rockMatrices[amountFlying + i] = rock;
		circlingRocks[i] = info;
	}
}
