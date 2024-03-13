#pragma once

#include <vector>
#include <objects/Vertex.h>

class TriangleFace {
private:
	const std::vector<float> calculateNormal();
	const float calculateDotProduct(const std::vector<float> v1, const std::vector<float> v2);
public:
	TriangleFace(std::vector<Vertex> vertices);
	std::vector<Vertex> vertices;
	bool isVisible(Vertex vantagePoint);
};
