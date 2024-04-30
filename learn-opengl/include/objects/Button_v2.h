#pragma once
#include <string>
#include <vector>
#include <objects/Renderable.h>
#include <objects/Text.h>
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

	/* These params are in normalized device format */
	Button_v2(float left, float right, float top, float bottom);

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

