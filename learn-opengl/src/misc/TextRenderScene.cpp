#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <misc/TextRenderScene.h>
#include <misc/Shaders.h>
#include <misc/FontManager.h>
#include <objects/Character.h>
#include <string>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

TextRenderScene::TextRenderScene() {
	m_VAO = 0;
	m_VBO = 0;
	m_shader = Shader(Shaders::getTextVertexShader(), Shaders::getTextFragmentShader());
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	// testing fonts
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		throw std::exception("ERROR::FREETYPE: Could not init FreeType Library");
	}

	FT_Face face;
	if (FT_New_Face(ft, FontManager::getFontPath().c_str(), 0, &face)) {
		throw std::exception("ERROR:FREETYPE: Failed to load font");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		throw std::exception("ERROR::FREETYPE: Failed to load Glyph");
	}

	for (unsigned char c = 0; c < 128; c++) {
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		//set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_characters.insert(std::pair<char, Character>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	// clear FreeType resources once finished processing the glyphs
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderScene::render(std::string text, float x, float y, float scale, glm::vec3 color) {
	getInstance().useShader();
	glUniform3f(glGetUniformLocation(m_shader.getID(), "textColor"), color.x, color.y, color.z);
	//glm::mat4 projection = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f);
	//glUniformMatrix4fv(glGetUniformLocation(m_shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = m_characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,		ypos + h,	0.0f, 0.0f },
			{ xpos,		ypos,		0.0f, 1.0f },
			{ xpos + w, ypos,		1.0f, 1.0f },

			{ xpos,		ypos + h,	0.0f, 0.0f },
			{ xpos + w, ypos,		1.0f, 1.0f },
			{ xpos + w, ypos + h,	1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderScene::setVBO(unsigned int vbo) {
	m_VBO = vbo;
}

void TextRenderScene::setVAO(unsigned int vao) {
	m_VAO = vao;
}

void TextRenderScene::useShader() {
	m_shader.use();
}
