#pragma once

#include <iostream>
#include <GL/glew.H>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Renderer
{
public:
	Renderer(unsigned int vao, unsigned int vbo, Shader *_shader);
	~Renderer();

	void renderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	unsigned int VAO, VBO;
	std::map<GLchar, Character> Characters;
	Shader *shader;
};

