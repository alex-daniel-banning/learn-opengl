#pragma once
#include <vector>
#include <objects/TriangleFace.h>

class Button {
private:
	std::vector<TriangleFace> faces;

public:
	Button(std::vector<Vertex> vertices, std::vector<int> vertexIndices);
	std::vector<float> generateVertexBufferData();
};
