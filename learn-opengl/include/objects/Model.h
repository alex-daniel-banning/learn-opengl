#pragma once

#include <objects/Vertex.h>
#include <objects/TriangleFace.h>
#include <vector>

class Model {
	
public:
	Model() = default;
	~Model() = default;
	Model(const Model& other);
	Model(Model&& other) noexcept;
	Model(const std::vector<TriangleFace> faces);
	Model& operator=(const Model& other);
	Model& operator=(Model&& other) noexcept;

	std::vector<float> generateVertexBufferData(const Vertex vantagePoint, float distanceFromScreen);

private:
	std::vector<TriangleFace> m_faces;
};
