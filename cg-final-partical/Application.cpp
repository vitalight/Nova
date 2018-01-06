#include "Application.h"

//#define A_LINE
#define A_LIGHT_POS (glm::vec3(0, 0, 0))
#define A_LIGHT_COL (glm::vec3(1.0f, 0.8f, 0.8f))
#define A_LIGHT_BIAS (glm::vec2(0.4f, 0.7f))
#define A_LINE_DIRECTION (glm::vec3(-1, -1, 0))

#define A_OCTAVES 3
#define A_AMPLITUDE 10
#define A_ROUGHNESS (0.35f)

#define A_TERRAIN_SIZE 100
#define A_TERRAIN_TICK 6
#define A_ASTEROID_NUM 800
#define A_ASTEROID_RADIUS 600

vector<glm::vec3> A_TERRAIN_COLS = { glm::vec3(201 / 255.0f, 178 / 255.0f, 99 / 255.0f),
								glm::vec3(135 / 255.0f, 184 / 255.0f, 82 / 255.0f),
								glm::vec3(120 / 255.0f, 120 / 255.0f, 120 / 255.0f),
								glm::vec3(200 / 255.0f, 200 / 255.0f, 210 / 255.0f) };
#define A_COLOR_SPREAD (0.45f)Vertex.glsl

Application::Application()
{
}

Application::~Application()
{
	delete textRenderer;
	delete light;
	delete skybox;
}

void Application::Init()
{		
	/*************************************************************
	 * Environment
	 *************************************************************/
	light = new Light(A_LIGHT_POS, A_LIGHT_COL, A_LIGHT_BIAS);
	camera = new Camera(glm::perspective(glm::radians(45.0f), (float)A_SCR_WIDTH / (float)A_SCR_HEIGHT, 0.1f, 2000.0f), glm::vec3(0.0f, 0.0f, 450.0f));

	/*************************************************************
	 * Compile shaders
	 *************************************************************/
	// sun light source
	ResourceManager::LoadShader("glsl/texture.vs", "glsl/sun.fs", nullptr, "sun");
	// texture and light
	ResourceManager::LoadShader("texture");
	// skybox texture
	ResourceManager::LoadShader("skybox");
	// asteroids shader (instance render)
	ResourceManager::LoadShader("asteroids");
	// used to check normal line
	//ResourceManager::LoadShader("glsl/normal.vs", "glsl/normal.fs", "glsl/normal.gs", "normal");

	/*************************************************************
	* Load models
	*************************************************************/
	ResourceManager::LoadModel("asteroids", "asteroids", glm::vec3(0, 0, 0));
	ResourceManager::LoadModel("sun", "sun", glm::vec3(0.5, -1, 0));
	ResourceManager::LoadModel("earth", "texture", glm::vec3(0.5, -1, 0));
	ResourceManager::LoadModel("moon", "sun", glm::vec3(0.5, -1, 0));
	ResourceManager::LoadModel("saturn", "texture", glm::vec3(0.5, -1, 0));
	ResourceManager::LoadModel("shuttle", "texture", glm::vec3(0.5, -1, 0));/*
	
	/*************************************************************
	* Entities
	*************************************************************/
	/*
	 * 太阳sun,			水星mercury,		金星vernus, 
	 * 地球earth,		月球moon,		火星mars, 
	 * 小行星带asteroid,	木星jupiter,		土星saturn, 
	 * 天王星uranus,		海王星neptune,	彗星pluto
	 */
	Entity *sun = new Entity("sun", glm::vec3(0, 0, 0), 50),
		*mars = new Entity("moon", glm::vec3(0, 0, 0), 5),
		*earth = new Entity("earth", glm::vec3(0, 0, 0), 4),
		*moon = new Entity("moon", glm::vec3(0, 0, 0), 0.8),
		*saturn = new Entity("saturn", glm::vec3(0, 0, 0), 3),
		*shuttle = new Entity("shuttle", glm::vec3(0, 0, 0), 0.05);

	// planet type
	sun->configPlanet(0.1);
	mars->configPlanet(0.5, 300, 0.2);
	earth->configPlanet(0.5, 360, 0.05);
	saturn->configPlanet(0.5, 410, 0.02);
	moon->configMoon(earth, 0.5, 20, 0.5);
	shuttle->configShuttle();

	entities.push_back(sun);
	entities.push_back(mars);
	entities.push_back(earth);
	entities.push_back(saturn);
	entities.push_back(moon);
	entities.push_back(shuttle);

	// asteroid partical
	asteroids = new Partical("asteroids", A_ASTEROID_NUM, getTime(), A_ASTEROID_RADIUS);

	// partical system
	particalManager = new ParticalManager("asteroids", "asteroids");

	// universe skybox
	skybox = new Skybox(ResourceManager::GetShader("skybox"));

	/*************************************************************
	* Text render
	*************************************************************/
	textRenderer = new TextRenderer(A_SCR_WIDTH, A_SCR_HEIGHT);
	textRenderer->Load("resources/fonts/arial.ttf", 24);
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
	asteroids->draw(*camera, *light, deltaTime);
	particalManager->draw(*camera, *light, deltaTime);

	// draw at last
	skybox->draw(*camera);

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

/*************************************************************
* Reference codes
*************************************************************/
