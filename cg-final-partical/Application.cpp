#include "Application.h"

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
	light = new Light(NV_LIGHT_POS, NV_LIGHT_COL, NV_LIGHT_BIAS);
	camera = new Camera(glm::perspective(glm::radians(45.0f), (float)NV_SCR_WIDTH / (float)NV_SCR_HEIGHT, 0.1f, 2500.0f), glm::vec3(0.0f, 0.0f, 600.0f));

	/*************************************************************
	 * Compile shaders
	 *************************************************************/
	// sun light source
	ResourceManager::LoadShader("glsl/texture.vs", "glsl/light.fs", nullptr, "light");
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
	ResourceManager::LoadModel("earth", "multitexture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("moon", "light", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("sun", "light", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("mercury", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("venus", "texture", glm::vec3(0.5, -0.5, 0));

	ResourceManager::LoadPlanetModel("mars", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("jupiter", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("saturn", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("uranus", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("neptune", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadModel("shuttle", "texture", glm::vec3(0));
	
	/*************************************************************
	* Entities
	*************************************************************/
	/* 公转,自传,公转半径,半径
	 * 太阳	sun-0-25-0,				 水星mercury-88-59-57-24,		金星venus-224-243-108-60, 
	 * 地球	earth-365-1-149-63,		 月球moon,						火星mars-687-1-227-33, 
	 * 小行星带asteroid,				 木星jupiter-11y-0.4-778-714,	土星saturn-29.5y-0.5-1427-603, 
	 * 天王星uranus-84y-0.8-2869-259, 海王星neptune-164y-0.6-4504-247
	 */

	entityManager.createEntity("sun", 100)->configPlanet(0.146);
	entityManager.createEntity("mercury", 4.8)->configPlanet(0.6, 560, 0.06);
	entityManager.createEntity("venus", 12)->configPlanet(0.6, 700, 0.016);
	Entity * earth = entityManager.createEntity("earth", 12.6);
	earth->configPlanet(3, 860, 0.01);
	entityManager.createEntity("moon", 1.6)->configMoon(earth, 0.2, 60, 0.5);

	entityManager.createEntity("mars", 6.6)->configPlanet(3, 1040, 0.005);
	entityManager.createEntity("jupiter", 20)->configPlanet(4, 1360, 0.002);
	entityManager.createEntity("saturn", 18)->configPlanet(5, 1600, 0.001);
	entityManager.createEntity("uranus", 14)->configPlanet(4, 1940, 0.005);
	entityManager.createEntity("neptune", 13)->configPlanet(5, 2240, 0.003);
	entityManager.createEntity("shuttle", 0.05)->configShuttle();

	// partical system
	particalManager = new ParticalManager("asteroids", "asteroids", NV_FLYING_NUM, NV_CIRCLING_NUM, NV_ROCK_RADIUS, NV_ROCK_OFFSET);

	// universe skybox
	skybox = new Skybox(ResourceManager::GetShader("skybox"));

	// text render
	textRenderer = new TextRenderer(NV_SCR_WIDTH, NV_SCR_HEIGHT);
	textRenderer->Load("resources/fonts/arial.ttf", 24);
}

void Application::Update()
{	
	// calculate eplased time
	float currentFrame = getTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processKeyboard();

	entityManager.draw(*light, *camera, deltaTime);
	particalManager->draw(*light, *camera, deltaTime);
	skybox->draw(*camera);

	// show text: fps
	showFPS();
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
