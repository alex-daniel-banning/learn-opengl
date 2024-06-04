#include "2D_Shapes/Cross.h"
#include <GLFW/glfw3.h>

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

void Cross::render(Shader& shader, glm::vec3 &position, float angleOfRotation, glm::vec3 &pivotPoint)
{
    glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, angleOfRotation, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, position);
	transform = glm::scale(transform, glm::vec3(scale, scale, scale));

    shader.setMat4("model", transform);
	shader.use();

	glBindVertexArray(VAO);
	glLineWidth(10.0f * scale);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

float Cross::getWidth()
{
	return scale;
}