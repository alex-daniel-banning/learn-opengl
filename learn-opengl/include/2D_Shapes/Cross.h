#pragma once
#include <glad/glad.h>

#include <Shader.h>

class Cross
{
public:
	static const float VERTICES[];

	Cross(const float scale);
	~Cross();

	void initialize();

	void render(Shader& shader, glm::vec3& position, float angleOfRotation, glm::vec3& pivotPoint);
	float getWidth();

private:
	unsigned int VBO, VAO;
	float scale;
};
