#pragma once
#include <render/Shader.h>
#include <string>
#include <objects/Character.h>
#include <map>

class Text {
public:
	Text();
	~Text() = default;
	Text(const Text& other);
	Text(Text&& other) noexcept;
	Text& operator=(const Text& other);
	Text& operator=(Text&& other) noexcept;
	void render();
private:
	Shader m_shader;
	std::string m_text;
	float m_x, m_y, m_scale;
	glm::vec3 m_color;
	unsigned int m_VAO, m_VBO;
};
