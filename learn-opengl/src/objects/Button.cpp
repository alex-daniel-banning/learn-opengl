#include <glad/glad.h>
#include <objects/Button.h>
#include <misc/Shaders.h>
#include <iostream>

// fix later, make default have resonable default values and generate vertices base on static constants
Button::Button() : m_VAO(0), m_VBO(0) {
	m_shader = Shader(Shaders::getModelVertexShader(), Shaders::getModelFragmentShader());
	m_text = Text();
	m_bufferData = {};
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_left = -0.5f;
	m_right = 0.5f;
	m_top = 0.5f;
	m_bottom = -0.5f;
	generateBufferData();

    // configure VAO/VBO for model
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Button::Button(const Button& other) {
	m_shader = Shader(other.m_shader);
	m_text = other.m_text;
	m_color = glm::vec3(other.m_color);
	m_bufferData = std::vector(other.m_bufferData);
	m_VBO = other.m_VBO;
	m_VAO = other.m_VAO;
	m_left = other.m_left;
	m_right = other.m_right;
	m_top = other.m_top;
	m_bottom = other.m_bottom;
}

Button& Button::operator=(const Button& other) {
	if (this != &other) {
		m_shader = Shader(other.m_shader);
		m_text = other.m_text;
		m_color = glm::vec3(other.m_color);
		m_bufferData = std::vector(other.m_bufferData);
		m_VBO = other.m_VBO;
		m_VAO = other.m_VAO;
		m_left = other.m_left;
		m_right = other.m_right;
		m_top = other.m_top;
		m_bottom = other.m_bottom;
	}
	return *this;
}

Button::Button(float left, float right, float top, float bottom, std::string text) {
	m_color = glm::vec3(0.7f, 0.7f, 0.7f);
	m_shader = Shader(Shaders::getModelVertexShader(), Shaders::getModelFragmentShaderConfigurableColor());
	m_left = left;
	m_right = right;
	m_top = top;
	m_bottom = bottom;
	m_text = Text(text, 800.0f, 600.0f, m_left, m_right, m_top, m_bottom, 1.0f);

	GLint colorLoc = glGetUniformLocation(m_shader.getID(), "color");
	m_shader.use();
	glUniform4f(colorLoc, m_color.x, m_color.y, m_color.z, 1.0f);

	generateBufferData();

    // configure VAO/VBO for model
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Button::render() const {
	m_shader.use();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindVertexArray(m_VAO);
	glBufferData(
		GL_ARRAY_BUFFER,
		m_bufferData.size() * sizeof(float),
		m_bufferData.data(),
		GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, m_bufferData.size());
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_text.render();
}

float Button::getLeft() {
	return m_left;
}

float Button::getRight() {
	return m_right;
}

float Button::getTop() {
	return m_top;
}

float Button::getBottom() {
	return m_bottom;
}

void Button::generateBufferData() {
	m_bufferData.clear();
	std::vector<glm::vec3> vertices = {
		glm::vec3(m_left,	m_top,		0.0f),
		glm::vec3(m_right,	m_top,		0.0f),
		glm::vec3(m_right,	m_bottom,	0.0f),
		glm::vec3(m_left,	m_bottom,	0.0f)
	};
	int indices[6] = {
		0,	1,	2,
		2,	3,	0
	};
	for (int i : indices) {
		glm::vec3 vertex = vertices[i];
		m_bufferData.push_back(vertex.x);
		m_bufferData.push_back(vertex.y);
		m_bufferData.push_back(vertex.z);
	}
}
