#pragma once

#include <objects/Vertex.h>
#include <objects/Face.h>
#include <vector>

class Model {
private:
	std::vector<Face> faces;
	
public:

	Model(std::vector<Face> faces) : faces(faces) {};
	std::vector<float> generateVertexBufferData(const Vertex vantagePoint, float distanceFromScreen);
};
