#include "Application.h"

#define LIGHT_POS (glm::vec3(0, 0, 0))
#define LIGHT_COL (glm::vec3(1.0f, 0.8f, 1.0f))
#define LIGHT_BIAS (glm::vec3(0.3f, 0.8f, 0.5f))
#define LINE_DIRECTION (glm::vec3(-1, -1, 0))

#define ASTEROID_FLYING 100
#define ASTEROID_CIRCLING 800
#define ASTEROID_RADIUS 600
#define ASTEROID_OFFSET 15.0f

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
	light = new Light(LIGHT_POS, LIGHT_COL, LIGHT_BIAS);
	camera = new Camera(glm::perspective(glm::radians(45.0f), (float)A_SCR_WIDTH / (float)A_SCR_HEIGHT, 0.1f, 2000.0f), glm::vec3(0.0f, 0.0f, 450.0f));

	/*************************************************************
	 * Compile shaders
	 *************************************************************/
	// sun light source
	ResourceManager::LoadShader("glsl/texture.vs", "glsl/sun.fs", nullptr, "sun");
	// texture and light
	ResourceManager::LoadShader("texture");
	// multitexture for earth
	ResourceManager::LoadShader("multitexture");
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
	ResourceManager::LoadModel("earth", "multitexture", glm::vec3(0.5, -1, 0));
	ResourceManager::LoadModel("moon", "sun", glm::vec3(0.5, -1, 0));
	//ResourceManager::LoadModel("resources/objects/saturn2/Saturn.max", "saturn", "texture", glm::vec3(0));
	ResourceManager::LoadModel("saturn", "texture", glm::vec3(0.5, -1, 0));
	ResourceManager::LoadModel("shuttle", "texture", glm::vec3(0));/*
	
	/*************************************************************
	* Entities
	*************************************************************/
	/* 公转,自传
	 * 太阳sun--25,				水星mercury-88-59,		金星vernus-224-243, 
	 * 地球earth-365-1,			月球moon,				火星mars-687-1, 
	 * 小行星带asteroid,			木星jupiter-11y-0.4,		土星saturn-29.5y-0.5, 
	 * 天王星uranus-84y-0.8,		海王星neptune-164y-0.6
	 */
	Entity *sun = new Entity("sun", glm::vec3(0), 50),
		*mercury = new Entity("moon", glm::vec3(0), 4),
		*vernus = new Entity("moon", glm::vec3(0), 4),
		*earth = new Entity("earth", glm::vec3(0), 6),
		*moon = new Entity("moon", glm::vec3(0), 0.8),

		*mars = new Entity("moon", glm::vec3(0), 7),
		*jupiter = new Entity("moon", glm::vec3(0), 2),
		*saturn = new Entity("saturn", glm::vec3(0), 3),
		*uranus = new Entity("moon", glm::vec3(0), 2),
		*neptune = new Entity("moon", glm::vec3(0), 2),
		*shuttle = new Entity("shuttle", glm::vec3(0), 0.05);

	// planet type
	sun->configPlanet(0.146);
	mercury->configPlanet(5.061, 300, 0.04);//0.061
	vernus->configPlanet(0.015, 400, 0.016);
	earth->configPlanet(3.65, 500, 0.01);
	moon->configMoon(earth, 0.2, 30, 0.5);// todo

	mars->configPlanet(3.65, 600, 0.005);
	jupiter->configPlanet(9.125, 700, 0.002);
	saturn->configPlanet(7.3, 800, 0.001);
	uranus->configPlanet(4.56, 900, 0.005);
	neptune->configPlanet(6.08, 1000, 0.003);
	shuttle->configShuttle();

	// put in vector
	entities.push_back(sun);
	entities.push_back(mercury);
	entities.push_back(vernus);
	entities.push_back(earth);
	entities.push_back(moon);

	entities.push_back(mars);
	entities.push_back(jupiter);
	entities.push_back(saturn);
	entities.push_back(uranus);
	entities.push_back(neptune);
	entities.push_back(shuttle);

	// partical system
	particalManager = new ParticalManager("asteroids", "asteroids", ASTEROID_FLYING, ASTEROID_CIRCLING, ASTEROID_RADIUS, ASTEROID_OFFSET);

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
