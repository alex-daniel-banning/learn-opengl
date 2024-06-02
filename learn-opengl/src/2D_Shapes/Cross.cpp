#include "2D_Shapes/Cross.h"

const float Cross::VERTICES[12] = {
	-0.5f,  0.0f, 0.0f,
	 0.5f,  0.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.0f, -0.5f, 0.0f
};

Cross::Cross(const float scale)
{
	this->scale = scale;
};

Cross::~Cross()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
};

void Cross::initialize()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
};

void Cross::render(Shader& shader)
{
	shader.use();

	glBindVertexArray(VAO);
	glLineWidth(5.0f * scale);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}