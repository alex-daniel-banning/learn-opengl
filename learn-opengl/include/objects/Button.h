#pragma once
#include <vector>
#include <string>
#include <objects/TriangleFace.h>
#include <objects/Renderable.h>
#include <render/Shader.h>
#include <glm/glm.hpp>

class Button { 
public:
	Button() = default;
	~Button() = default;
	Button(const Button& other);
	Button(Button&& other) noexcept;
	Button& operator=(const Button& other);
	Button& operator=(Button&& other) noexcept;

	Button(std::vector<Vertex> vertices, std::vector<int> vertexIndices);
	std::vector<float> generateVertexBufferData();

private:
	std::vector<TriangleFace> faces;
};

