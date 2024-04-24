#pragma once

#include <misc/Shaders.h>

const char* Shaders::getModelVertexShader() {
	return 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
}

const char* Shaders::getModelFragmentShader() {
	return
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
}

const char* Shaders::getModelFragmentShaderConfigurableColor() {
	return
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 color;\n"
		"void main()\n"
		"{\n"
		"  FragColor = color;\n"
		"}\n\0";
}

const char* Shaders::getTextVertexShader() {
	return
		"#version 330 core\n"
		"layout (location = 0) in vec4 vertex;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
		"  TexCoords = vertex.zw;\n"
		"}\n\0";
}

const char* Shaders::getTextFragmentShader() {
	return
		"#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D text;\n"
		"uniform vec3 textColor;\n"
		"void main()\n"
		"{\n"
		"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
		"    color = vec4(textColor, 1.0) * sampled;\n"
		"}\n\0";
}
