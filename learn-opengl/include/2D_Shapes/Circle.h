#pragma once
#include <glad/glad.h>

#include <Shader.h>

class Circle
{
public:
	static const float VERTICES[]; // todo keep?

	Circle(const float radius);
	~Circle();

	void initialize();

	void render(Shader& shader);
	void render(Shader& shader, glm::vec3& position, float angleOfRotation, glm::vec3& pivotPoint);

private:
	unsigned int VBO, VAO;
	float radius;
	std::vector<float> vertices;
};
