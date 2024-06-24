#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <GL\glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <RenderEngine/Shader.h>

#include <map>
#include <iostream>



struct Character {
	uint32 TextureID;   // ID handle of the glyph texture
	glm::vec2 Size;    // Size of glyph
	glm::vec2 Bearing;  // Offset from baseline to left/top of glyph
	uint32 Advance;    // Horizontal offset to advance to next glyph
};


class Text
{

public:
	Text();
	~Text();
	void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	std::map<char, Character> Characters;
	uint32 VAO, VBO;
};








#endif // !TEXT_H
