#include <objects/TriangleFace.h>
#include <objects/Vertex.h>
#include <cassert>

TriangleFace::TriangleFace(const TriangleFace& other) {
	for (Vertex vertex : other.vertices) {
		vertices.push_back(Vertex(vertex));
	}
}

TriangleFace::TriangleFace(TriangleFace&& other) noexcept {
	vertices = std::move(other.vertices);
	other.vertices.clear();
}

TriangleFace& TriangleFace::operator=(const TriangleFace& other) {
	if (this != &other) {
		vertices.clear();
		for (const Vertex vertex : other.vertices) {
			vertices.push_back(Vertex(vertex));
		}
	}
	return *this;
}

TriangleFace& TriangleFace::operator=(TriangleFace&& other) noexcept {
	if (this != &other) {
		vertices = std::move(other.vertices);
		other.vertices.clear();
	}
	return *this;
}

TriangleFace::TriangleFace(std::vector<Vertex> vertices)
	: vertices(vertices) {
	assert(vertices.size() == 3);
}

bool TriangleFace::isVisible(Vertex vantagePoint) {
	const std::vector<float> normal = calculateNormal();
	Vertex pointOnFace = vertices[0];
	const std::vector<float> eyelineVector = {
		pointOnFace.x - vantagePoint.x,
		pointOnFace.y - vantagePoint.y,
		pointOnFace.z - vantagePoint.z
	};
	const float dotProduct = calculateDotProduct(normal, eyelineVector);
	return dotProduct < 0;
};

const std::vector<float> TriangleFace::calculateNormal() {
	std::vector<float> v1 = {
		vertices[1].x - vertices[0].x,
		vertices[1].y - vertices[0].y,
		vertices[1].z - vertices[0].z
	};
	
	std::vector<float> v2 = {
		vertices[2].x - vertices[1].x,
		vertices[2].y - vertices[1].y,
		vertices[2].z - vertices[1].z
	};

	const std::vector<float> result = {
		(v1[1] * v2[2]) - (v1[2] * v2[1]),
		(v1[2] * v2[0]) - (v1[0] * v2[2]),
		(v1[0] * v2[1]) - (v1[1] * v2[0])
	};
	return result;
};

const float TriangleFace::calculateDotProduct(const std::vector<float> v1, const std::vector<float> v2) {
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}
