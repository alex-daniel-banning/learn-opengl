#include <glad/glad.h> // Must include this before GLFW/glfw3.h. The GLFW depends on stuff from glad.
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <objects/Vertex.h>
#include <objects/TriangleFace.h>
#include <objects/Model.h>
#include <objects/Button.h>
#include <misc/Shaders.h>
#include <windows.h>
#include <shobjidl.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
bool buttonWasClicked(GLFWwindow* window, double xPos, double yPos);

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* setupGLFW();
unsigned int setupOrangeShaderProgram();
std::vector<TriangleFace> generateFaces(std::vector<Vertex> vertices, std::vector<int> vertexIndices);
std::vector<TriangleFace> generateCubeFaces();
std::vector<TriangleFace> generateTriangularPrismFaces();
std::vector<TriangleFace> generateTetrahedronFaces();
Button createButton();

int main() {

	GLFWwindow* window;
	try {
		window = setupGLFW();
	}
	catch (const std::exception& e) {
		std::cerr << "There was an error upon GLFW setup." << std::endl;
		return -1;
	}
	unsigned int shaderProgramOrange = setupOrangeShaderProgram();

	/***/
	/***/

	/* Tetrahedron is working, yout just have to view it from the right angle (vantage point 0, 0, 0)*/
	Model model(generateTetrahedronFaces());
	std::vector<float> modelBufferData = model.generateVertexBufferData(Vertex(0.0f, 0.0f, 0.0f), 1.0f);
	Button button = createButton();
	std::vector<float> buttonBufferData = button.generateVertexBufferData();

	std::vector<float> bufferData;
	bufferData.reserve(modelBufferData.size() + buttonBufferData.size());
	bufferData.insert(bufferData.end(), modelBufferData.begin(), modelBufferData.end());
	bufferData.insert(bufferData.end(), buttonBufferData.begin(), buttonBufferData.end());

	unsigned int VBOs[1], VAOs[1];
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), bufferData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// no need to unbind, as we are about to bind a new VAO

	// turn on wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, bufferData.size());
		
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteProgram(shaderProgramOrange);

	glfwTerminate();
	return 0;
}

/*************************************************/

GLFWwindow* setupGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw std::exception();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::exception();
	}

	glViewport(0, 0, 600, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	return window;
}

unsigned int setupOrangeShaderProgram() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = Shaders::getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderOrange;
	fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = Shaders::getFragmentShaderOrangeSource();
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderOrange);
	glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgramOrange;
	shaderProgramOrange = glCreateProgram();
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);
	glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::ORANGE::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);

	return shaderProgramOrange;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "mouse has been clicked at position -> " << xpos << ":" << ypos << std::endl;
		if (buttonWasClicked(window, xpos, ypos)) {
			//std::cout << "button clicked" << std::endl;

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
								MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
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

bool buttonWasClicked(GLFWwindow* window, double xPos, double yPos) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	// if x is in between button max and min x
	double buttonMinX, buttonMaxX, buttonMinY, buttonMaxY;
	buttonMinX = (-0.9f * (width) / 2.0f) + ((width) / 2.0f);
	buttonMaxX = (-0.8f * (width) / 2.0f) + ((width) / 2.0f);
	buttonMinY = (-0.9f * (height) / 2.0f) + ((height) / 2.0f);
	buttonMaxY = (-0.8f * (height) / 2.0f) + ((height) / 2.0f);

	return xPos > buttonMinX && xPos < buttonMaxX && yPos > buttonMinY && yPos < buttonMaxY;
}

std::vector<TriangleFace> generateFaces(std::vector<Vertex> vertices, std::vector<int> vertexIndices) {
	std::vector<TriangleFace> faces;

	for (int vertexIndex = 0; vertexIndex < vertexIndices.size(); vertexIndex += 3) {
		std::vector<Vertex> faceVertices;
		for (int i = 0; i < 3; i++) {
			faceVertices.push_back(vertices[vertexIndices[vertexIndex + i]]);
		}
		TriangleFace face(faceVertices);
		faces.push_back(face);
	}
	return faces;
}

std::vector<TriangleFace> generateCubeFaces() {
	std::vector<Vertex> vertices = {
		{ -0.5f, -0.5f,  2.5f },
		{ -0.5f,  0.5f,  2.5f },
		{  0.5f,  0.5f,  2.5f },
		{  0.5f, -0.5f,  2.5f },
		{ -0.5f, -0.5f,  3.5f },
		{ -0.5f,  0.5f,  3.5f },
		{  0.5f,  0.5f,  3.5f },
		{  0.5f, -0.5f,  3.5f }
	};
	std::vector<int> vertexIndices = {
		0, 1, 2,  2, 3, 0,
		7, 6, 5,  5, 4, 7,
		4, 5, 1,  1, 0, 4,
		3, 2, 6,  6, 7, 3,
		1, 5, 6,  6, 2, 1,
		3, 7, 4,  4, 0, 3
	};
	return generateFaces(vertices, vertexIndices);
}

std::vector<TriangleFace> generateTriangularPrismFaces() {
	std::vector<Vertex> vertices = {
		{ -0.5f, -0.5f,  2.5f },
		{  0.0f,  0.5f,  2.5f },
		{  0.5f, -0.5f,  2.5f },
		{ -0.5f, -0.5f,  3.5f },
		{  0.0f,  0.5f,  3.5f },
		{  0.5f, -0.5f,  3.5f }
	};
	std::vector<int> vertexIndices = {
		0, 1, 2,
		3, 5, 4,
		2, 1, 4, 4, 5, 2,
		3, 4, 1, 1, 0, 3,
		0, 2, 5, 5, 3, 0
	};
	return generateFaces(vertices, vertexIndices);
}

std::vector<TriangleFace> generateTetrahedronFaces() {
	std::vector<Vertex> vertices = {
		{  0.5f, -0.5f,  2.5f },
		{ -0.5f,  0.5f,  2.5f },
		{ -0.5f, -0.5f,  3.5f },
		{  0.5f,  0.5f,  3.5f }
	};
	std::vector<int> vertexIndices = {
		0, 3, 2,
		0, 2, 1,
		1, 2, 3,
		0, 1, 3
	};
	return generateFaces(vertices, vertexIndices);
}

Button createButton() {
	std::vector<Vertex> vertices = {
		{ -0.9f, 0.9f, 1.0f },
		{ -0.8f,  0.9f, 1.0f },
		{ -0.8f,  0.8f,  1.0f },
		{ -0.9f, 0.8f,  1.0f }
	};
	std::vector<int> vertexIndices = {
		0, 1, 2,
		2, 3, 0
	};
	return Button(vertices, vertexIndices);
}
