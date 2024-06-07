#define _USE_MATH_DEFINES
#include <math.h>

#include "2D_Shapes/Circle.h"

Circle::Circle(const float radius)
{
	VAO = 0;
	VBO = 0;
	this->vertices = std::vector<float>();
	this->radius = radius;
}

Circle::~Circle()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Circle::initialize()
{ 
	int NUM_SEGMENTS = 1000 * radius;
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	for (int i = 0; i <= NUM_SEGMENTS; i++)
	{
		float angle = 2.0f * M_PI * i / NUM_SEGMENTS;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(1.0f);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Circle::render(Shader& shader)
{
    glm::mat4 transform = glm::mat4(1.0f);
    shader.setMat4("model", transform);

	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
	glBindVertexArray(0);
}

void Circle::render(Shader& shader, glm::vec3 &position, float angleOfRotation, glm::vec3 &pivotPoint)
{
    glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, angleOfRotation, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, position);

    shader.setMat4("model", transform);
	shader.use();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
	glBindVertexArray(0);
}
