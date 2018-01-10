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
	camera = new Camera(glm::perspective(glm::radians(45.0f), (float)NV_SCR_WIDTH / (float)NV_SCR_HEIGHT, 0.1f, 2500.0f), 
						glm::vec3(0.0f, 0.0f, NV_CAMERA_SPEED));

	/*************************************************************
	 * Compile shaders
	 *************************************************************/
	// shader for text
	ResourceManager::LoadShader("text");
	// ugly shader is needed for show difference
	ResourceManager::LoadShader("ugly");
	// sun light source
	ResourceManager::LoadShader("glsl/texture.vs", "glsl/light.fs", nullptr, "light");
	// texture and light
	ResourceManager::LoadShader("texture");
	// multitexture for earth
	ResourceManager::LoadShader("multitexture");
	// skybox texture
	ResourceManager::LoadShader("skybox");
	// asteroids shader
	ResourceManager::LoadShader("asteroids");
	// shader for fire animation
	ResourceManager::LoadShader("fire");
	// used to show normal line
	//ResourceManager::LoadShader("glsl/normal.vs", "glsl/normal.fs", "glsl/normal.gs", "normal");

	/*************************************************************
	* Load models
	*************************************************************/
	ResourceManager::LoadModel("myrobot", "asteroids", glm::vec3(0));
	ResourceManager::LoadModel("asteroids", "asteroids", glm::vec3(0, 0, 0));
	ResourceManager::LoadModel("earth", "multitexture", glm::vec3(0.5, -0.5, 0));
	// for test
	ResourceManager::LoadPlanetModel("sun", "light", glm::vec3(0.5, -0.5, 0))->brightness = 1.0f;
	ResourceManager::LoadPlanetModel("moon", "texture", glm::vec3(0.5, -0.5, 0));
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
	earth = entityManager.createEntity("earth", 12.6);
	earth->configPlanet(2, 860, 0.01);
	entityManager.createEntity("moon", 1.6)->configMoon(earth, 0.2, 60, 1.0);

	entityManager.createEntity("mars", 6.6)->configPlanet(3, 1040, 0.005);
	entityManager.createEntity("jupiter", 20)->configPlanet(4, 1360, 0.002);
	entityManager.createEntity("saturn", 18)->configPlanet(5, 1600, 0.001);
	entityManager.createEntity("uranus", 14)->configPlanet(4, 1940, 0.005);
	entityManager.createEntity("neptune", 13)->configPlanet(5, 2240, 0.003);
	shuttle = entityManager.createEntity("shuttle", 0.05);
	shuttle->configShuttle(*camera);

	// partical system
	particalManager = new ParticalManager(shuttle, "asteroids", NV_FLYING_NUM, NV_CIRCLING_NUM, NV_ROCK_RADIUS, NV_ROCK_OFFSET);

	// universe skybox
	skybox = new Skybox(ResourceManager::GetShader("skybox"));

	// text render
	textRenderer = new TextRenderer("text", NV_SCR_WIDTH, NV_SCR_HEIGHT);
	textRenderer->Load("resources/fonts/STXIHEI.TTF", 30);
}

void Application::Update()
{	
	// calculate eplased time
	float currentFrame = getTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processKeyboard();

	entityManager.draw(*light, *camera, deltaTime);
	skybox->draw(*camera);
	particalManager->draw(*light, *camera, deltaTime);

	// show text: fps
	showFPS();
}

void Application::SwitchShader()
{
	earth->switchShader();
}

void Application::SwitchPartical()
{
	particalManager->switchPartical();
}

float Application::getTime()
{
	return glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

void Application::processKeyboard()
{
	// process movement
	bool hasKey = false;
	if (GetKeyState('W') < 0) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
		shuttle->position += camera->Front * deltaTime * camera->MovementSpeed;
		particalManager->generateFire(*camera, camera->Front, camera->Right, camera->Up);
		hasKey = true;
	}
	if (GetKeyState('S') < 0) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
		shuttle->position -= camera->Front * deltaTime * camera->MovementSpeed;
		particalManager->generateFire(*camera, -camera->Front, camera->Right, camera->Up);
		hasKey = true;
	}
	if (GetKeyState('A') < 0) {
		camera->ProcessKeyboard(LEFT, deltaTime);
		shuttle->position -= camera->Right * deltaTime * camera->MovementSpeed;
		if (!hasKey)
			particalManager->generateFire(*camera, -camera->Right + camera->Front, camera->Front, camera->Up);
		hasKey = true;
	}
	if (GetKeyState('D') < 0) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
		shuttle->position += camera->Right * deltaTime * camera->MovementSpeed;
		if (!hasKey)
			particalManager->generateFire(*camera, camera->Right + camera->Front, camera->Front, camera->Up);
		hasKey = true;
	}
	if (GetKeyState('Q') < 0) {
		camera->ProcessKeyboard(UP, deltaTime);
		shuttle->position += camera->Up * deltaTime * camera->MovementSpeed;
		if (!hasKey)
			particalManager->generateFire(*camera, camera->Up + camera->Front, camera->Right, camera->Front);
	}
	if (GetKeyState('E') < 0) {
		camera->ProcessKeyboard(DOWN, deltaTime);
		shuttle->position -= camera->Up * deltaTime * camera->MovementSpeed;
		if (!hasKey)
			particalManager->generateFire(*camera, -camera->Up + camera->Front, camera->Right, camera->Front);
	}

	static float brightness = 1.0f;
	// process brightness adjustment
	if (GetKeyState('Z') < 0 && brightness < 1.0) {
		brightness += deltaTime * NV_LIGHT_ADJUST;
		ResourceManager::GetModel("sun")->brightness = brightness;
		light->Color = NV_LIGHT_COL * brightness;
	}
	if (GetKeyState('X') < 0 && brightness > 0.3) {
		brightness -= deltaTime * NV_LIGHT_ADJUST;
		ResourceManager::GetModel("sun")->brightness = brightness;
		light->Color = NV_LIGHT_COL * brightness;
	}
}

void Application::processMouse(int xoffset, int yoffset)
{
	camera->ProcessMouseMovement(xoffset, yoffset);
	shuttle->angle = -glm::radians(camera->Yaw) + PI;
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
