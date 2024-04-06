#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <shobjidl.h>
#include <string>
#include <objects/Model.h>
#include <objects/Button.h>

class ModelRenderScene {
public:
	static ModelRenderScene& getInstance() {
		static ModelRenderScene instance;
		return instance;
	}

	ModelRenderScene(const ModelRenderScene&) = delete;
	ModelRenderScene(ModelRenderScene&&) = delete;
	ModelRenderScene& operator=(const ModelRenderScene&) = delete;
	ModelRenderScene& operator=(ModelRenderScene&&) = delete;

	void handleMouseInput(GLFWwindow* window, int button, int action, int mods);
	std::vector<float> getVertexBufferData();

private:
	Button m_selectModelButton;
	Model m_mainModel;
	Vertex m_vantagePoint;
	float m_distanceFromScreen;
	std::vector<float> m_vertexBufferData;

	ModelRenderScene();

	bool buttonWasClicked(GLFWwindow* window, double xPos, double yPos);
	void processFileImport(PWSTR filePath);
	std::vector<float> generateVertexBufferData();
};
