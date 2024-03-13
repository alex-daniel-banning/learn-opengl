#pragma once

#include <objects/Vertex.h>
#include <objects/TriangleFace.h>
#include <vector>

class Model {
private:
	std::vector<TriangleFace> faces;
	
public:

	Model(std::vector<TriangleFace> faces) : faces(faces) {};
	std::vector<float> generateVertexBufferData(const Vertex vantagePoint, float distanceFromScreen);
};
