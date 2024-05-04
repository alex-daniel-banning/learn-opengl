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
	bool isVisible(Vertex vantagePoint, float rotation, float zTranslation);

private:
	const std::vector<float> calculateNormal();
	const std::vector<float> calculateNormal(float rotation, float zTranslation);
	const float calculateDotProduct(const std::vector<float> v1, const std::vector<float> v2);
};
