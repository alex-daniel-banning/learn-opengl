#pragma once
#include <render/Shader.h>
#include <objects/Character.h>
#include <string>
#include <map>

class TextRenderScene {
public:
	static TextRenderScene& getInstance() {
		static TextRenderScene instance;
		return instance;
	}

	TextRenderScene(const TextRenderScene&) = delete;
	TextRenderScene(TextRenderScene&&) = delete;
	TextRenderScene& operator=(const TextRenderScene&) = delete;
	TextRenderScene& operator=(TextRenderScene&&) = delete;

	void setVBO(unsigned int vbo);
	void setVAO(unsigned int vao);
	void render(std::string text, float x, float y, float scale, glm::vec3 color);
private:
	unsigned int m_VBO, m_VAO;
	Shader m_shader;
	std::map<char, Character> m_characters;

	TextRenderScene();
	void useShader();
};
