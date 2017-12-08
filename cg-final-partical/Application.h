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
#include "Entity.h"
#include "Terrain.h"

#define A_SCR_HEIGHT 600
#define A_SCR_WIDTH 1000

class Application {
public:
	Camera *camera;

	Application();
	void Init();
	void Update();
private:
	TextRenderer *textRenderer;
	Light *light;
	vector<Entity*> entities;

	float deltaTime, lastTime = 0;
	unsigned int frameCnt = 0, fps = 0;
	float currentFrame = 0,
		lastFrame = 0;

	void drawEntity();
	void showFPS();

	float getTime();
	void processKeyboard();


	// for partical presentation
	Model *rock;
	void prepareAsteroids();
	void drawAsteroids();
};