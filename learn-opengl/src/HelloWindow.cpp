#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <glad/glad.h> // Must include this before GLFW/glfw3.h. The GLFW depends on stuff from glad.
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <windows.h>
#include <GLFW/glfw3native.h>
#include <misc/Shaders.h>
#include <misc/ModelRenderScene.h>
#include <misc/FontManager.h>

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

struct Character {
	unsigned int	TextureID;
	glm::ivec2		Size;
	glm::ivec2		Bearing;
	unsigned int	Advance;
};

void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);
void RenderScene();

int main() {

	GLFWwindow* window;
	try {
		window = setupGLFW();
	}
	catch (const std::exception& e) {
		std::cerr << "There was an error upon GLFW setup." << std::endl;
		return -1;
	}

	// testing fonts
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, FontManager::getFontPath().c_str(), 0, &face)) {
		std::cout << "ERROR:FREETYPE: Failed to load font" << std::endl;
		return -1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
		return -1;
	}
	
	std::map<char, Character> Characters;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++) {
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		//set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	// clear FreeType resources once finished processing the glyphs
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	unsigned int shaderProgramOrange = setupOrangeShaderProgram();

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glUseProgram(shaderProgramOrange);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
	// this can probably be better organized/refactored
	ModelRenderScene::getInstance().setVBO(VBOs[0]);
	ModelRenderScene::getInstance().setVAO(VAOs[0]);
	// no need to unbind, as we are about to bind a new VAO

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		RenderScene();
		
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
	const char* vertexShaderSource = Shaders::getModelVertexShader();
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
	const char* fragmentShaderSource = Shaders::getModelFragmentShader();
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

// do I have to set the shader for the text and the models each time before rendering? -> ChatGPT says Yes
// todo add Shader &s as first param and use it as it's used in https://learnopengl.com/In-Practice/Text-Rendering
//		Will need to create a shader class
void RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {

}

void RenderScene() {
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, ModelRenderScene::getInstance().getVBO());
	//glBindVertexArray(VAOs[0]);
	glBindVertexArray(ModelRenderScene::getInstance().getVAO());
	glBufferData(
		GL_ARRAY_BUFFER,
		ModelRenderScene::getInstance().getVertexBufferData().size() * sizeof(float),
		ModelRenderScene::getInstance().getVertexBufferData().data(),
		GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, ModelRenderScene::getInstance().getVertexBufferData().size());
}
