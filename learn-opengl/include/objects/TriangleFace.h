#pragma once

#include <vector>
#include <objects/Vertex.h>

class TriangleFace {
public:

	TriangleFace() = default;
	~TriangleFace() = default;
	TriangleFace(const TriangleFace& other);
	TriangleFace(TriangleFace&& other) noexcept;
	TriangleFace& operator=(const TriangleFace& other);
	TriangleFace& operator=(TriangleFace&& other) noexcept;

	TriangleFace(std::vector<Vertex> vertices);
	std::vector<Vertex> vertices;
	bool isVisible(Vertex vantagePoint);

private:
	const std::vector<float> calculateNormal();
	const float calculateDotProduct(const std::vector<float> v1, const std::vector<float> v2);
};
