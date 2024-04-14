#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <glad/glad.h> // Must include this before GLFW/glfw3.h. The GLFW depends on stuff from glad.
#include <GLFW/glfw3.h>
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <windows.h>
#include <GLFW/glfw3native.h>
#include <misc/Shaders.h>
#include <misc/ModelRenderScene.h>
#include <misc/TextRenderScene.h>
#include <misc/FontManager.h>
#include <render/Shader.h>

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

int main() {

	GLFWwindow* window;
	try {
		window = setupGLFW();
	}
	catch (const std::exception& e) {
		std::cerr << "There was an error upon GLFW setup." << std::endl;
		return -1;
	}

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	/*
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
	// this can probably be better organized/refactored
	ModelRenderScene::getInstance().setVBO(VBOs[0]);
	ModelRenderScene::getInstance().setVAO(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// no need to unbind, as we are about to bind a new VAO
	*/

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	TextRenderScene::getInstance().setVBO(VBOs[1]);
	TextRenderScene::getInstance().setVAO(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//ModelRenderScene::getInstance().render();
		TextRenderScene::getInstance().render("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	ModelRenderScene::getInstance().handleMouseInput(window, button, action, mods);
}
