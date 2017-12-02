#pragma once
//#pragma comment(lib,"glew32.lib")
#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Shader.h"
#include "util/stb_image.h"

class Application {
public:
	Application();
	void run();
private:
	int shaderProgram;
	unsigned int VAO, texture1, texture2;
	Shader *ourShader;
};