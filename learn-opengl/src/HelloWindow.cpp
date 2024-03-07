#include <glad/glad.h> // Must include this before GLFW/glfw3.h. The GLFW depends on stuff from glad.
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "../include/objects/Vertex.h"
#include "../include/objects/QuadFace.h"

/* Assignment
		Given a set of faces for a cube, render the appropriate faces.
		Create the set of vertices.
*/

std::vector<int> getVisibleFaceIndices(QuadFace(&faces)[6]);
bool isVisible(QuadFace& face);
std::vector<float> createVectorFromVertexPair(const Vertex& v1, const Vertex& v2);
std::vector<float> calculateCrossProduct(std::vector<float> a, std::vector<float> b);
float calculateDotProduct(std::vector<float> &a, std::vector<float> &b);

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

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

const Vertex VANTAGE_POINT = Vertex(0.0f, 0.0f, 0.0f);

int main() {

	/* GLFW setup begins */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	glViewport(0, 0, 800, 600);
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

	Vertex vertices[8] = {
		Vertex(-0.5f, -0.5f, 2.5),
		Vertex(-0.5f, 0.5f, 2.5),
		Vertex(0.5f, 0.5f, 2.5),
		Vertex(0.5f, -0.5f, 2.5),
		Vertex(-0.5f, -0.5f, 3.5),
		Vertex(-0.5f, 0.5f, 3.5),
		Vertex(0.5f, 0.5f, 3.5),
		Vertex(0.5f, -0.5f, 3.5)
	};
	QuadFace faces[6] = {
		QuadFace({ vertices[0], vertices[1], vertices[2], vertices[3] }),
		QuadFace({ vertices[7], vertices[6], vertices[5], vertices[1] }),
		QuadFace({ vertices[4], vertices[5], vertices[1], vertices[0] }),
		QuadFace({ vertices[3], vertices[2], vertices[6], vertices[0] }),
		QuadFace({ vertices[1], vertices[5], vertices[6], vertices[2] }),
		QuadFace({ vertices[3], vertices[7], vertices[4], vertices[0] }),
	};
	
	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.45, 0.5f, 0.0f
	};

	float cube[] = {-0.5f, -0.5f ,0.0f, -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f,
					0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};

	unsigned int VBOs[1], VAOs[1];
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);
	// first triangle setup
	// --------------------
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
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
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
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

std::vector<int> getVisibleFaceIndices(QuadFace(&faces)[6]) {
	std::vector<int> visibleFaceIndices;

	for (int i = 0; i < 6; i++) {
		if (isVisible(faces[i])) {
			visibleFaceIndices.push_back(i);
		}
	}

	return visibleFaceIndices;
}

bool isVisible(QuadFace& face) {

	// 1. Get the cross product of the face
	const Vertex* faceVertices = face.getVertices();
	std::vector<float> v1 = createVectorFromVertexPair(faceVertices[1], faceVertices[0]);
	std::vector<float> v2 = createVectorFromVertexPair(faceVertices[2], faceVertices[1]);
	std::vector<float> crossProductVector = calculateCrossProduct(v1, v2);

	// 2. Get the dot product of the cross product above and the eye-to-point vector	
	std::vector<float> vantagePointVector = createVectorFromVertexPair(VANTAGE_POINT, faceVertices[0]);
	float dotProductValue = calculateDotProduct(crossProductVector, vantagePointVector);

	// 3. Return true if result is negative
	return dotProductValue < 0;
}

std::vector<float> createVectorFromVertexPair(const Vertex& v1, const Vertex& v2) {
	return { v2.getX() - v1.getX(), v2.getY() - v1.getY(), v2.getZ() - v1.getZ() };
}

std::vector<float> calculateCrossProduct(std::vector<float> a, std::vector<float> b) {
	return {
		(a[1] * b[2]) - (a[2] * b[1]),
		(a[2] * b[0]) - (a[0] * b[2]),
		(a[0] * b[1]) - (a[1] * b[0])
	};
}

float calculateDotProduct(std::vector<float> &a, std::vector<float> &b) {
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}
