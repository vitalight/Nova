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
	ResourceManager::LoadPlanetModel("moon", "resources/objects/planetTextures/2k_moon.jpg", "light", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("sun", "resources/objects/planetTextures/2k_sun.jpg", "light", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("mercury", "resources/objects/planetTextures/2k_mercury.jpg", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("venus", "resources/objects/planetTextures/2k_venus_surface.jpg", "texture", glm::vec3(0.5, -0.5, 0));

	ResourceManager::LoadPlanetModel("mars", "resources/objects/planetTextures/2k_mars.jpg", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("jupiter", "resources/objects/planetTextures/2k_jupiter.jpg", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("saturn", "resources/objects/planetTextures/2k_saturn.jpg", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("uranus", "resources/objects/planetTextures/2k_uranus.jpg", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadPlanetModel("neptune", "resources/objects/planetTextures/2k_neptune.jpg", "texture", glm::vec3(0.5, -0.5, 0));
	ResourceManager::LoadModel("shuttle", "texture", glm::vec3(0));
	
	/*************************************************************
	* Entities
	*************************************************************/
	/* 公转,自传,公转半径,半径
	 * 太阳	sun-0-25-0,				水星mercury-88-59-57-24,		金星venus-224-243-108-60, 
	 * 地球	earth-365-1-149-63,		月球moon,					火星mars-687-1-227-33, 
	 * 小行星带asteroid,				木星jupiter-11y-0.4-778-714,	土星saturn-29.5y-0.5-1427-603, 
	 * 天王星uranus-84y-0.8-2869-259,海王星neptune-164y-0.6-4504-247
	 */
	Entity *sun = new Entity("sun", glm::vec3(0), 100),
		*mercury = new Entity("mercury", glm::vec3(0), 4.8),
		*venus = new Entity("venus", glm::vec3(0), 12),
		*earth = new Entity("earth", glm::vec3(0), 12.6),
		*moon = new Entity("moon", glm::vec3(0), 1.6),

		*mars = new Entity("mars", glm::vec3(0), 6.6),
		*jupiter = new Entity("jupiter", glm::vec3(0), 20),
		*saturn = new Entity("saturn", glm::vec3(0), 18),
		*uranus = new Entity("uranus", glm::vec3(0), 14),
		*neptune = new Entity("neptune", glm::vec3(0), 13),
		*shuttle = new Entity("shuttle", glm::vec3(0), 0.05);

	// planet type
	sun->configPlanet(0.146);
	mercury->configPlanet(0.6, 560, 0.05);
	venus->configPlanet(0.6, 700, 0.016);
	earth->configPlanet(3, 860, 0.01);
	moon->configMoon(earth, 0.2, 60, 0.5);// todo

	mars->configPlanet(3, 1040, 0.005);
	jupiter->configPlanet(4, 1360, 0.002);
	saturn->configPlanet(5, 1600, 0.001);
	uranus->configPlanet(4, 1940, 0.005);
	neptune->configPlanet(5, 2240, 0.003);
	shuttle->configShuttle();

	// put in vector
	entities.push_back(sun);
	entities.push_back(mercury);
	entities.push_back(venus);
	entities.push_back(earth);
	entities.push_back(moon);

	entities.push_back(mars);
	entities.push_back(jupiter);
	entities.push_back(saturn);
	entities.push_back(uranus);
	entities.push_back(neptune);
	entities.push_back(shuttle);

	// partical system
	particalManager = new ParticalManager("asteroids", "asteroids", NV_FLYING_NUM, NV_CIRCLING_NUM, NV_ROCK_RADIUS, NV_ROCK_OFFSET);

	// universe skybox
	skybox = new Skybox(ResourceManager::GetShader("skybox"));

	/*************************************************************
	* Text render
	*************************************************************/
	textRenderer = new TextRenderer(NV_SCR_WIDTH, NV_SCR_HEIGHT);
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
