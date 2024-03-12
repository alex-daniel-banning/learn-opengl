#include <glad/glad.h> // Must include this before GLFW/glfw3.h. The GLFW depends on stuff from glad.
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <objects/Vertex.h>
#include <objects/TriangleFace.h>
#include <objects/Model.h>

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::vector<TriangleFace> generateFaces(std::vector<Vertex> vertices, std::vector<int> vertexIndices);
std::vector<TriangleFace> generateCubeFaces();
std::vector<TriangleFace> generateTriangularPrismFaces();
std::vector<TriangleFace> generateTetrahedronFaces();

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderOrangeSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main() {

	/* GLFW setup begins */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 600, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	/* GLFW setup ends */

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderOrangeSource, NULL);
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

	/****************************** End of Setup **********************************/

	/* New stuff here */
	
	// todo, get rid of Cube class and clean up remnants
	//Model cubeModel(generateCubeFaces());
	//std::vector<float> cubeBufferData = cubeModel.generateVertexBufferData(Vertex(1.5f, 1.5f, 0.0f), 1.0f);
	//Model trianglularPrismModel(generateTriangularPrismFaces());
	//std::vector<float> triangularPrismBufferData = trianglularPrismModel.generateVertexBufferData(Vertex(1.5f, 1.5f, 0.0f), 1.0f);
	//Model tetrahedronModel(generateTetrahedronFaces());
	//std::vector<float> tetrahedronBufferData = tetrahedronModel.generateVertexBufferData(Vertex(1.5f, 1.5f, 0.0f), 1.0f);

	/* Tetrahedron is working, yout just have to view it from the right angle (vantage point 0, 0, 0)*/
	Model model(generateTetrahedronFaces());
	std::vector<float> modelBufferData = model.generateVertexBufferData(Vertex(0.0f, 0.0f, 0.0f), 1.0f);
	/******************/

	unsigned int VBOs[1], VAOs[1];
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, modelBufferData.size() * sizeof(float), modelBufferData.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// no need to unbind, as we are about to bind a new VAO

	// turn on wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, modelBufferData.size());
		
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
