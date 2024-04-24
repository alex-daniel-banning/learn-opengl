#include <glad/glad.h>
#include <objects/Button_v2.h>
#include <misc/Shaders.h>

// fix later, make default have resonable default values and generate vertices base on static constants
Button_v2::Button_v2() : m_VAO(0), m_VBO(0) {
	m_shader = Shader(Shaders::getTextVertexShader(), Shaders::getTextFragmentShader());
	m_text = "Button_v2";
	m_bufferData = {};
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	// todo setup vao/vbo/bufferdata
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

Button_v2::Button_v2(const Button_v2& other) {
	m_shader = Shader(other.m_shader);
	m_text = other.m_text;
	m_color = glm::vec3(other.m_color);
	m_bufferData = std::vector(other.m_bufferData);
	m_VBO = other.m_VBO;
	m_VAO = other.m_VAO;
}

Button_v2& Button_v2::operator=(const Button_v2& other) {
	if (this != &other) {
		m_shader = Shader(other.m_shader);
		m_text = other.m_text;
		m_color = glm::vec3(other.m_color);
		m_bufferData = std::vector(other.m_bufferData);
		m_VBO = other.m_VBO;
		m_VAO = other.m_VAO;
	}
	return *this;
}

Button_v2::Button_v2(float left, float right, float top, float bottom) {
	m_text = "Button 2!";
	m_color = glm::vec3(0.7f, 0.7f, 0.7f);
	m_shader = Shader(Shaders::getModelVertexShader(), Shaders::getModelFragmentShaderConfigurableColor());

	GLint colorLoc = glGetUniformLocation(m_shader.getID(), "color");
	m_shader.use();
	glUniform4f(colorLoc, m_color.x, m_color.y, m_color.z, 1.0f);

	std::vector<glm::vec3> vertices = {
		glm::vec3(left,	top, 0.0f),
		glm::vec3(right, top, 0.0f),
		glm::vec3(right, bottom, 0.0f),
		glm::vec3(left, bottom, 0.0f)
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

void Button_v2::render() {
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
}
