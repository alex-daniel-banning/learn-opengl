#pragma once

class Shaders {
public:
	static const char* getModelVertexShader();
	static const char* getModelFragmentShader();
	static const char* getModelFragmentShaderConfigurableColor();
	static const char* getTextVertexShader();
	static const char* getTextFragmentShader();
};
