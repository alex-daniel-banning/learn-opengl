#pragma once

#include <vector>
#include <objects/Vertex.h>

class Face {
private:
	const std::vector<float> calculateNormal();
	const float calculateDotProduct(const std::vector<float> v1, const std::vector<float> v2);
public:
	Face(std::vector<Vertex> vertices) : vertices(vertices) {};
	std::vector<Vertex> vertices;
	bool isVisible(Vertex vantagePoint);
};
