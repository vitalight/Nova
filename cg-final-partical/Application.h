#pragma once
#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "includes/stb_image.h"
#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#define A_SCR_HEIGHT 600
#define A_SCR_WIDTH 1000

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Application {
public:
	Camera camera;
	float deltaTime, lastTime = 0;

	Application();
	void init();
	void run();
private:
	// positions all containers
	glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[4] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Shader *ourShader;
	Model *ourModel;

	int shaderProgram;
	unsigned int VAO, VBO, cubeVAO, lightVAO, diffuseMap, specularMap, frameCnt = 0, fps = 0;
	float currentFrame = 0,
		lastFrame = 0;
	std::map<GLchar, Character> Characters;

	void renderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void showFPS();
	float getTime();
	void processKeyboard();
	unsigned int loadTexture(char const * path);
};