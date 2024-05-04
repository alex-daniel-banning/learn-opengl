#pragma once
#include <string>
#include <vector>
#include <objects/Renderable.h>
#include <objects/Text.h>
#include <render/Shader.h>
#include <glm/glm.hpp>

class Button : public Renderable {
public:
	Button();
	~Button() = default;
	Button(const Button& other);
	Button(Button&& other) noexcept = default;
	Button& operator=(const Button& other);
	Button& operator=(Button&& other) noexcept = default;

	/* These params are in normalized device format */
	Button(float left, float right, float top, float bottom, std::string text);

	void render() const;

	float getLeft();
	float getRight();
	float getTop();
	float getBottom();

private:
	Shader m_shader;
	Text m_text;
	std::vector<float> m_bufferData;
	glm::vec3 m_color;
	unsigned int m_VBO, m_VAO;
	float m_left, m_right, m_top, m_bottom;
	void generateBufferData();
};

