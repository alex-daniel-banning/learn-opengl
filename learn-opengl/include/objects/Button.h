#pragma once
#include <vector>
#include <objects/TriangleFace.h>

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

