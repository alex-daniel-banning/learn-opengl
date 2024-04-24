#pragma once
#include <string>
#include <vector>
#include <objects/Renderable.h>
#include <render/Shader.h>
#include <glm/glm.hpp>

class Button_v2 : public Renderable {
public:
	Button_v2();
	~Button_v2() = default;
	Button_v2(const Button_v2& other);
	Button_v2(Button_v2&& other) noexcept = default;
	Button_v2& operator=(const Button_v2& other);
	Button_v2& operator=(Button_v2&& other) noexcept = default;

	Button_v2(float left, float right, float top, float bottom);

	void render();

private:
	Shader m_shader;
	std::string m_text;
	std::vector<float> m_bufferData;
	glm::vec3 m_color;
	unsigned int m_VBO, m_VAO;
};

