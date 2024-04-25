#pragma once

#include <objects/Text.h>
#include <objects/Character.h>
#include <misc/Characters.h>

Text::Text() : m_VAO(0), m_VBO(0) {
	m_shader = Shader(Shaders::getTextVertexShader(), Shaders::getTextFragmentShader());
	// ----
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    m_shader.use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// ----
	m_text = "pee pee poo poo";
	m_x = 25.0f;
	m_y = 25.0f;
	m_scale = 0.5f;
	m_color = glm::vec3(0.3, 0.7f, 0.9f);
	initializeGLComponents();
}

Text::Text(const Text& other) {
	// copy members
	m_shader = Shader(other.m_shader); // not 100% sure this makes sense because it would duplicate the instance of an ID
	m_text = other.m_text;
	m_x = other.m_x;
	m_y = other.m_y;
	m_scale = other.m_scale;
	m_color = glm::vec3(other.m_color);
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}

Text::Text(Text&& other) noexcept {
	m_shader = std::move(other.m_shader);
	m_text = std::move(other.m_text);
	m_x = other.m_x;
	m_y = other.m_y;
	m_scale = other.m_scale;
	m_color = std::move(other.m_color);
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}

Text& Text::operator=(const Text& other) {
	m_shader = Shader(other.m_shader);
	m_text = other.m_text;
	m_x = other.m_x;
	m_y = other.m_y;
	m_scale = other.m_scale;
	m_color = glm::vec3(other.m_color);
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	return *this;
}

Text& Text::operator=(Text&& other) noexcept {
	m_shader = std::move(other.m_shader);
	m_text = std::move(other.m_text);
	m_x = other.m_x;
	m_y = other.m_y;
	m_scale = other.m_scale;
	m_color = std::move(other.m_color);
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	return *this;
}

Text::Text(	const std::string text,
			const float viewportWidth,
			const float viewportHeight,
			const float normalizedLeft,
			const float normalizedRight,
			const float normalizedTop,
			const float normalizedBottom,
			const float scale) : m_VAO(0), m_VBO(0) {
	m_shader = Shader(Shaders::getTextVertexShader(), Shaders::getTextFragmentShader());
	// ----
    glm::mat4 projection = glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight);
    m_shader.use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// ----
	m_text = text;
	//m_x = normalizedLeft * viewportWidth;
	m_x = (viewportWidth / 2) + ((viewportWidth / 2) * normalizedLeft);
	//m_y = normalizedBottom * viewportHeight;
	m_y = (viewportHeight / 2) + ((viewportHeight / 2) * normalizedBottom);
	m_scale = scale;
	m_color = glm::vec3(0.0f, 0.0f, 0.0f);
	initializeGLComponents();
}

void Text::render() const {
	m_shader.use();
	glUniform3f(glGetUniformLocation(m_shader.getID(), "textColor"), m_color.x, m_color.y, m_color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	float temp_x = m_x;
	// iterate through all characters
	std::string::const_iterator c;
	for (c = m_text.begin(); c != m_text.end(); c++) {
		Character ch = Characters::get(*c);

		float xpos = temp_x + ch.Bearing.x * m_scale;
		float ypos = m_y - (ch.Size.y - ch.Bearing.y) * m_scale;

		float w = ch.Size.x * m_scale;
		float h = ch.Size.y * m_scale;
		// update VBO for each character
		float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
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
		temp_x += (ch.Advance >> 6) * m_scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::initializeGLComponents() {
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0); // not sure what this does
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

