#include "Application.h"

// this enables selecting high performance graphics card
//extern "C" {
//	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//}

//#define A_LINE
#define A_LIGHT_DIR (glm::vec3(-1.0f, -10.0f, -10.0f))
#define A_LIGHT_COL (glm::vec3(1.0f, 0.9f, 0.9f))
#define A_LIGHT_BIAS (glm::vec2(0.8f, 0.2f))
#define A_LINE_DIRECTION (glm::vec3(-1, -1, 0))

#define A_OCTAVES 3
#define A_AMPLITUDE 10
#define A_ROUGHNESS (0.35f)

#define A_TERRAIN_SIZE 100
#define A_TERRAIN_TICK 6


vector<glm::vec3> A_TERRAIN_COLS = { glm::vec3(201 / 255.0f, 178 / 255.0f, 99 / 255.0f),
								glm::vec3(135 / 255.0f, 184 / 255.0f, 82 / 255.0f),
								glm::vec3(120 / 255.0f, 120 / 255.0f, 120 / 255.0f),
								glm::vec3(200 / 255.0f, 200 / 255.0f, 210 / 255.0f) };
#define A_COLOR_SPREAD (0.45f)

Application::Application()
{
}

void Application::Init()
{	
	// environment
	light = new Light(A_LIGHT_DIR, A_LIGHT_COL, A_LIGHT_BIAS);
	camera = new Camera(glm::perspective(glm::radians(45.0f), (float)A_SCR_WIDTH / (float)A_SCR_HEIGHT, 0.1f, 1000.0f), glm::vec3(0.0f, 0.0f, 300.0f));

	// shader
	ResourceManager::LoadShader("glsl/basicVertex.glsl", "glsl/basicFragment.glsl", nullptr, "basic");
	ResourceManager::LoadShader("glsl/textureVertex.glsl", "glsl/textureFragment.glsl", nullptr, "texture");
	ResourceManager::LoadShader("glsl/terrainVertex.glsl", "glsl/terrainFragment.glsl", nullptr, "terrain");
	ResourceManager::LoadShader("glsl/normalVertex.glsl", "glsl/normalFragment.glsl", "glsl/normalGeometry.glsl", "normal");

	// model
	ResourceManager::LoadModel("resources/objects/planet/planet.obj", "planet", "texture", glm::vec3(0.5, -1, 0));
	//ResourceManager::LoadModel("resources/objects/earth/source/123.blend", "earth", "texture", glm::vec3(0, 0, 0));
	//ResourceManager::LoadModel("resources/objects/rock/rock.obj", "rock", "texture");

	// entity
	/*
	 * 太阳sun,			水星mercury,		金星vernus, 
	 * 地球earth,		月球moon,		火星mars, 
	 * 小行星带asteroid,	木星jupiter,		土星saturn, 
	 * 天王星uranus,		海王星neptune,	彗星pluto
	 */
	Entity *sun = new Entity("planet", glm::vec3(0, 0, 0), 20),
		*planet1 = new Entity("planet", glm::vec3(0, 0, 0), 5),
		*earth = new Entity("planet", glm::vec3(0, 0, 0), 4),
		*moon = new Entity("planet", glm::vec3(0, 0, 0), 0.8),
		*saturn = new Entity("planet", glm::vec3(0, 0, 0), 3);

	// planet type
	sun->configPlanet(0.1);
	planet1->configPlanet(0.5, 120, 0.4);
	earth->configPlanet(0.5, 180, 0.2);
	saturn->configPlanet(0.5, 230, 0.1);
	moon->configMoon(earth, 0.5, 20, 2);

	entities.push_back(sun);
	entities.push_back(planet1);
	entities.push_back(earth);
	entities.push_back(saturn);
	entities.push_back(moon);

	// text
	textRenderer = new TextRenderer(A_SCR_WIDTH, A_SCR_HEIGHT);
	textRenderer->Load("resources/fonts/arial.ttf", 24);

#ifdef A_LINE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
}

void Application::Update()
{
	// per-frame logic
	// --------------------
	
	// calculate eplased time
	float currentFrame = getTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processKeyboard();

	drawEntity(deltaTime);

	// show text: fps
	showFPS();
}

void Application::drawEntity(float time)
{
	for (auto entity:entities)
		entity->Draw(*(this->light), *(this->camera), time);
}

float Application::getTime()
{
	return glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

void Application::processKeyboard()
{
	if (GetKeyState('W') < 0) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (GetKeyState('S') < 0) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (GetKeyState('A') < 0) {
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (GetKeyState('D') < 0) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
	if (GetKeyState('Q') < 0) {
		camera->ProcessKeyboard(UP, deltaTime);
	}
	if (GetKeyState('E') < 0) {
		camera->ProcessKeyboard(DOWN, deltaTime);
	}
}

void Application::showFPS()
{
	frameCnt++;
	if (lastFrame - lastTime > 1)
	{
		//cout << "gotcha" << endl;
		fps = frameCnt / (lastFrame - lastTime);
		lastTime = lastFrame;
		frameCnt = 0;
	}
	textRenderer->RenderText("FPS: " + to_string(fps), 25.0f, 25.0f, 0.6f, glm::vec3(1, 1, 1));

}

void Application::prepareAsteroids()
{
	ResourceManager::LoadShader("glsl/asteroids.vs", "glsl/asteroids.fs", nullptr, "asteroids");
	rock = new Model(ResourceManager::GetShader("asteroids"), "resources/objects/rock/rock.obj");
	unsigned int amount = 80000;

	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(getTime()); // initialize random seed	
	float radius = 100.0;
	float offset = 15.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock->meshes.size(); i++)
	{
		unsigned int VAO = rock->meshes[i].VAO;
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

void Application::drawAsteroids()
{
	unsigned int amount = 80000;
	// draw meteorites
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)A_SCR_WIDTH / (float)A_SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = camera->GetViewMatrix();
	ResourceManager::GetShader("asteroids").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("asteroids").SetMatrix4("view", view);
	ResourceManager::GetShader("asteroids").SetInteger("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rock->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < rock->meshes.size(); i++)
	{
		glBindVertexArray(rock->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
}