#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <objects/Renderable.h>
#include <objects/Button_v2.h>
#include <GLFW/glfw3.h>
#include <misc/ModelFileInputReader.h>
#include <misc/StringConverter.h>
#include <misc/Shaders.h>
#include <exceptions/ModelImportException.h>
#include <shobjidl_core.h>

class Scene {
public:
	static Scene& getInstance() {
		static Scene instance;
		return instance;
	}

	static void render() {
		getInstance().m_selectModelButton.render();

		//Model a = getInstance().m_mainModel;
		getInstance().m_mainModel.setVantagePoint(getInstance().m_vantagePoint);
		getInstance().m_mainModel.setDistanceFromScreen(getInstance().m_distanceFromScreen);
		//Model b = getInstance().m_mainModel;
		getInstance().m_mainModel.render();
	}

	static void handleInput(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			//std::cout << "mouse has been clicked at position -> " << xpos << ":" << ypos << std::endl;
			if (selectModelButtonWasClicked(window, xpos, ypos)) {
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

private:
	Button_v2 m_selectModelButton;
	Model m_mainModel;
	Shader m_modelShader;
	Vertex m_vantagePoint;
	float m_distanceFromScreen;

	Scene() {
		m_distanceFromScreen = 1.0f;
		m_vantagePoint = Vertex(0.0f, 0.0f, 0.0f);
		m_selectModelButton = Button_v2(-0.95f, -0.6f, 0.95f, 0.85f);
		m_mainModel = ModelFileInputReader::readModelFromFile("C:\\Users\\banni\\source\\repos\\learn-opengl\\resources\\models\\cube.model");
		m_modelShader = Shader(Shaders::getModelVertexShader(), Shaders::getModelFragmentShader());
	}

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	static bool selectModelButtonWasClicked(GLFWwindow* window, double xPos, double yPos) {
		int viewportWidth, viewportHeight;
		glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
		double minX, maxX, minY, maxY, widthMidpoint, heightMidpoint;
		widthMidpoint = viewportWidth / 2.0f;
		heightMidpoint = viewportHeight / 2.0f;
		minX = (getInstance().m_selectModelButton.getLeft() * widthMidpoint) + widthMidpoint;
		maxX = (getInstance().m_selectModelButton.getRight() * widthMidpoint) + widthMidpoint;
		maxY = viewportHeight - ((getInstance().m_selectModelButton.getBottom() * heightMidpoint) + heightMidpoint);
		minY = viewportHeight - ((getInstance().m_selectModelButton.getTop() * heightMidpoint) + heightMidpoint);
		return xPos > minX && xPos < maxX && yPos > minY && yPos < maxY;
	}
	static void processFileImport(PWSTR filePath) {
		try {
			getInstance().m_mainModel = ModelFileInputReader::readModelFromFile(filePath);
			getInstance().m_mainModel.setShader(getInstance().m_modelShader);
		}
		catch (const ModelImportException& e) {
			MessageBoxW(NULL, StringConverter::convertToWideChar(e.what()).get(), L"An error occured.", MB_OK);
		}
		catch (const std::exception& e) {
			MessageBoxW(NULL, StringConverter::convertToWideChar(e.what()).get(), L"An error occured.", MB_OK);
		}
	}
};
