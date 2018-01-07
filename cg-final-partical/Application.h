#pragma once
#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "TextRenderer.h"
#include "Light.h"
#include "Skybox.h"
#include "ParticalManager.h"
#include "Config.h"
#include "EntityManager.h"
//#include "Entity.h"

class Application {
public:
	Camera *camera;

	Application();
	~Application();
	void Init();
	void Update();
private:
	TextRenderer *textRenderer;
	Light *light;
	//vector<Entity*> entities;
	Skybox *skybox;
	ParticalManager *particalManager;
	EntityManager entityManager;

	float deltaTime, lastTime = 0;
	unsigned int frameCnt = 0, fps = 0;
	float currentFrame = 0,
		lastFrame = 0;

	void drawEntity(float time);
	void showFPS();

	float getTime();
	void processKeyboard();
};