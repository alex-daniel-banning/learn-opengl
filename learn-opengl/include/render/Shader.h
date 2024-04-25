#pragma once

class Shader {
public:
	Shader();
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~Shader();
	Shader(const Shader& other);
	Shader(Shader&& other) noexcept = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept;

	void use() const;
	unsigned int getID() const;

private:
	unsigned int ID;
};
