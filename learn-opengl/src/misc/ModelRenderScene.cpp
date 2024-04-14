#pragma once
#include <misc/ModelRenderScene.h>
#include <misc/ModelFileInputReader.h>
#include <misc/Shaders.h>
#include <misc/StringConverter.h>
#include <exceptions/ModelImportException.h>
#include <render/Shader.h>

ModelRenderScene::ModelRenderScene() {
	const std::vector<Vertex> buttonVertices = {
		{ -0.9f, 0.9f, 1.0f },
		{ -0.8f, 0.9f, 1.0f },
		{ -0.8f, 0.8f, 1.0f },
		{ -0.9f, 0.8f, 1.0f }
	};
	const std::vector<int> buttonVertexIndices = {
		0, 1, 2,
		2, 3, 0
	};
	Button button(buttonVertices, buttonVertexIndices);

	Model emptyModel;

	m_selectModelButton = button;
	m_mainModel = emptyModel;
	m_vantagePoint = {0.0f, 0.0f, 0.0f};
	m_distanceFromScreen = 1.0f;
	m_vertexBufferData = generateVertexBufferData();

	m_shader = Shader(Shaders::getModelVertexShader(), Shaders::getModelFragmentShader());
}

std::vector<float> ModelRenderScene::generateVertexBufferData() {
	std::vector<float> buttonBufferData = m_selectModelButton.generateVertexBufferData();
	std::vector<float> modelBufferData = m_mainModel.generateVertexBufferData(m_vantagePoint, m_distanceFromScreen);
	std::vector<float> combinedBufferData;
	combinedBufferData.reserve(buttonBufferData.size() + modelBufferData.size());
	combinedBufferData.insert(combinedBufferData.end(), buttonBufferData.begin(), buttonBufferData.end());
	combinedBufferData.insert(combinedBufferData.end(), modelBufferData.begin(), modelBufferData.end());
	return combinedBufferData;
}

void ModelRenderScene::handleMouseInput(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		//std::cout << "mouse has been clicked at position -> " << xpos << ":" << ypos << std::endl;
		if (buttonWasClicked(window, xpos, ypos)) {
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(hr)) {
				IFileOpenDialog* pFileOpen;
				 // Create the FileOpenDialog object.
				hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
				if (SUCCEEDED(hr)) {
					// Show the Open dialog box.
					hr = pFileOpen->Show(NULL);
					if (SUCCEEDED(hr)) {
						IShellItem* pItem;
						hr = pFileOpen->GetResult(&pItem);
						if (SUCCEEDED(hr)) {
							PWSTR pszFilePath;
							hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
							// Display the file name to the user.
							if (SUCCEEDED(hr)) {
								//MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
								processFileImport(pszFilePath);
								CoTaskMemFree(pszFilePath);
							}
							pItem->Release();
						}
					}
				}

				pFileOpen->Release();
			}
			CoUninitialize();
		}
	}
}

bool ModelRenderScene::buttonWasClicked(GLFWwindow* window, double xPos, double yPos) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	double minX, maxX, minY, maxY;
	minX = (-0.9f * width / 2.0f) + (width / 2.0f);
	maxX = (-0.8f * width / 2.0f) + (width / 2.0f);
	minY = (-0.9f * height / 2.0f) + (height / 2.0f);
	maxY = (-0.8f * height / 2.0f) + (height / 2.0f);

	return xPos > minX && xPos < maxX && yPos > minY && yPos < maxY;
}

void ModelRenderScene::processFileImport(PWSTR filePath) {
	try {
		m_mainModel = ModelFileInputReader::readModelFromFile(filePath);
	}
	catch (const ModelImportException& e) {
		MessageBoxW(NULL, StringConverter::convertToWideChar(e.what()).get(), L"An error occured.", MB_OK);
	}
	catch (const std::exception& e) {
		MessageBoxW(NULL, StringConverter::convertToWideChar(e.what()).get(), L"An error occured.", MB_OK);
	}
	m_vertexBufferData = generateVertexBufferData();
}

std::vector<float> ModelRenderScene::getVertexBufferData() {
	return m_vertexBufferData;
}

unsigned int ModelRenderScene::getVBO() {
	return m_VBO;
}

void ModelRenderScene::setVBO(unsigned int vbo) {
	m_VBO = vbo;
}

unsigned int ModelRenderScene::getVAO() {
	return m_VAO;
}

void ModelRenderScene::setVAO(unsigned int vao) {
	m_VAO = vao;
}

void ModelRenderScene::useShader() {
	m_shader.use();
}

void ModelRenderScene::render() {
	ModelRenderScene::getInstance().useShader();
	glBindBuffer(GL_ARRAY_BUFFER, ModelRenderScene::getInstance().getVBO());
	glBindVertexArray(ModelRenderScene::getInstance().getVAO());
	glBufferData(
		GL_ARRAY_BUFFER,
		ModelRenderScene::getInstance().getVertexBufferData().size() * sizeof(float),
		ModelRenderScene::getInstance().getVertexBufferData().data(),
		GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, ModelRenderScene::getInstance().getVertexBufferData().size());

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

