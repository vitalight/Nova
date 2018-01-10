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

/***************************************************************
* [Application]
*
*	The core class that invokes other modules to implement the
*	basic logic. Load in the resources(models, shaders, textures)
*	and update each item's position and render them on screen. 
****************************************************************/
class Application {
public:
	Camera *camera;

	Application();
	~Application();
	void Init();
	void Update();
	void SwitchShader();
	void SwitchPartical();
	void processMouse(int xoffset, int yoffset);
private:
	TextRenderer *textRenderer;
	Light *light;
	Skybox *skybox;
	ParticalManager *particalManager;
	EntityManager entityManager;
	/*************************************************
	* This two is list out of EntityManager because
	*	[Earth] will change shader according to user input
	*	[shuttle] ?
	**************************************************/
	Entity *earth, *shuttle;

	float deltaTime, lastTime = 0;
	unsigned int frameCnt = 0, fps = 0;
	float currentFrame = 0,
		lastFrame = 0;


	void showFPS();

	float getTime();
	void processKeyboard();
};