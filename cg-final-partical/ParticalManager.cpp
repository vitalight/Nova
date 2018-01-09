#include "ParticalManager.h"

#define USE_FIRE

ParticalManager::ParticalManager(string name, int _amountFlying, int _amountCircling, float radius, float offset)
{
	amountFlying = _amountFlying;
	amountCircling = _amountCircling;
	amountFireMax = 1000;

	// rock partical
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
	fire = ResourceManager::LoadFireModel("fire", "resources/atlas/fire.png");
	fireMatrices = new glm::mat4[amountFireMax];
	glGenBuffers(1, &fireVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glBufferData(GL_ARRAY_BUFFER, amountFireMax*sizeof(glm::mat4), &fireMatrices[0], GL_STREAM_DRAW);
	setupModel(fire);
#endif
}

void ParticalManager::setupModel(Model *model)
{
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

void ParticalManager::draw(Light &light, Camera &camera, float &time)
{
	update(time, camera);

	drawModel(rock, amountFlying + amountCircling, light, camera, time);
#ifdef USE_FIRE
	generateFire(camera);
	if (particalFires.size()) {
		drawModel(fire, particalFires.size(), light, camera, time);
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
	// todo
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

// todo: remove camera
void ParticalManager::update(const float time, Camera &camera)
{
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
	glBufferData(GL_ARRAY_BUFFER, (amountFlying + amountCircling) * sizeof(glm::mat4), &rockMatrices[0], GL_STREAM_DRAW);
#ifdef USE_FIRE
	// fire particals
	for (int i = 0; i < particalFires.size(); i++) {
		glm::mat4 fireMat;
		particalFires[i].position += time * particalFires[i].velocity;
		particalFires[i].elapseTime += time;
		if (particalFires[i].elapseTime > liveRange) {
			particalFires.erase(particalFires.begin() + i);
			i--;
			continue;
		}
		fireMat = glm::translate(fireMat, glm::vec3(particalFires[i].position));
		fireMatrices[i] = fireMat;
	}
	glBindBuffer(GL_ARRAY_BUFFER, fireVBO);
	glBufferData(GL_ARRAY_BUFFER, amountFireMax*sizeof(glm::mat4), &fireMatrices[0], GL_STREAM_DRAW);
#endif
}

void ParticalManager::generateFire(Camera &camera)
{
	if (particalFires.size() >= amountFireMax)
		return;
	ParticalFire part;
	part.velocity = -10.0f * camera.Front + float(rand() % 10 - 5) * camera.Right;// rand();
	part.position = camera.Position + camera.Front*30.0f - camera.Up*4.0f;// todo? change to a class
	particalFires.push_back(part);

	//cout << "new fire #" << particalFires.size() << endl;
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
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		info.angle = (float)i / (float)amountCircling * 360.0f;
		info.radius = radius + (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		info.y = ((rand() % (int)(2 * offset * 100)) / 100.0f - offset)*0.4;
		float x = sin(info.angle) * info.radius;
		float z = cos(info.angle) * info.radius;
		float y = info.y;
		rock = glm::translate(rock, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.25 and 1.25f
		info.scale = (rand() % 20) / 5.0f + 0.2f;
		rock = glm::scale(rock, glm::vec3(info.scale));
		
		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		info.rotAngle = (rand() % 360);
		rock = glm::rotate(rock, info.rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		rockMatrices[amountFlying + i] = rock;
		circlingRocks[i] = info;
	}
}
