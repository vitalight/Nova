#include "Application.h"

//#define A_LINE

unsigned int amount = 10000;

Application::Application()
	:camera(glm::vec3(0.0f, 0.0f, 155.0f))
{
}

Application::~Application()
{
	// to delete
}

void Application::Init()
{
	// Load shaders
	//ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	
	// Configure shaders
	//glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(A_SCR_WIDTH), static_cast<GLfloat>(A_SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
	//ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	//ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	
	// Load textures
	//ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	
	// Set render-specific controls
	//spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	/*=========================================================================================================*/
	ResourceManager::LoadShader("glsl/asteroids.vs", "glsl/asteroids.fs", nullptr, "asteroids");
	ResourceManager::LoadShader("glsl/planet.vs", "glsl/planet.fs", nullptr, "planet");

	rock = new Model("resources/objects/rock/rock.obj", ResourceManager::GetShader("asteroids"));
	planet = new Model("resources/objects/planet/planet.obj", ResourceManager::GetShader("planet"));

	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(getTime()); // initialize random seed	
	float radius = 150.0;
	float offset = 25.0f;
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

	/*=========================================================================================================*/
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
	processKeyboard();

	/*=========================================================================================================*/
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)A_SCR_WIDTH / (float)A_SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	ResourceManager::GetShader("asteroids").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("asteroids").SetMatrix4("view", view);

	ResourceManager::GetShader("planet").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("planet").SetMatrix4("view", view);

	// draw planet
	planet->Draw(glm::vec3(0, -20, 0) ,glm::vec3(15, 15, 15), getTime()/10.0, glm::vec3(0, 1, 0));

	// draw meteorites
	ResourceManager::GetShader("asteroids").Use().SetInteger("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rock->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < rock->meshes.size(); i++)
	{
		glBindVertexArray(rock->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
	/*=========================================================================================================*/

	showFPS();
}

float Application::getTime()
{
	return glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

void Application::processKeyboard()
{
	float currentFrame = getTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (GetKeyState('W') < 0) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (GetKeyState('S') < 0) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (GetKeyState('A') < 0) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (GetKeyState('D') < 0) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (GetKeyState('Q') < 0) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (GetKeyState('E') < 0) {
		camera.ProcessKeyboard(DOWN, deltaTime);
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