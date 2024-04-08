#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <glad/glad.h> // Must include this before GLFW/glfw3.h. The GLFW depends on stuff from glad.
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <windows.h>
#include <GLFW/glfw3native.h>
#include <misc/Shaders.h>
#include <misc/ModelRenderScene.h>

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* setupGLFW();
unsigned int setupOrangeShaderProgram();

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

	unsigned int VBOs[1], VAOs[1];
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(shaderProgramOrange);
	//// no need to unbind, as we are about to bind a new VAO

	// turn on wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBufferData(
			GL_ARRAY_BUFFER,
			ModelRenderScene::getInstance().getVertexBufferData().size() * sizeof(float),
			ModelRenderScene::getInstance().getVertexBufferData().data(),
			GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, ModelRenderScene::getInstance().getVertexBufferData().size());
		
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
	ModelRenderScene::getInstance().handleMouseInput(window, button, action, mods);
}

