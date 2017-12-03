#pragma once
#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "includes/stb_image.h"
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"

#define A_SCR_HEIGHT 1000
#define A_SCR_WIDTH 600

class Application {
public:
	Camera camera;
	float deltaTime;

	Application();
	void init();
	void run();
private:
	Shader *lightingShader,
		*lampShader;

	int shaderProgram;
	unsigned int cubeVAO, lightVAO, diffuseMap, specularMap;
	float currentFrame = 0,
		lastFrame = 0;

	float getTime();
	void processKeyboard();
	unsigned int loadTexture(char const * path);
};