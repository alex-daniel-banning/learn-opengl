#pragma once

#include <vector>
#include <utility>
#include <objects/Model.h>
#include <objects/Vertex.h>

Model::Model(const Model& other) {
	for (TriangleFace face : other.m_faces) {
		m_faces.push_back(TriangleFace(face));
	}
}

Model::Model(Model&& other) noexcept : m_faces(std::move(other.m_faces)) {
	other.m_faces.clear();
}

Model::Model(const std::vector<TriangleFace> faces) {
	for (const TriangleFace face : faces) {
		m_faces.push_back(TriangleFace(face));
	}
}

Model& Model::operator=(const Model& other) {
	if (this != &other) {
		m_faces.clear();
		for (const TriangleFace face : other.m_faces) {
			m_faces.push_back(TriangleFace(face));
		}
	}
	return *this;
}

Model& Model::operator=(Model&& other) noexcept {
	if (this != &other) {
		m_faces = std::move(other.m_faces);
		other.m_faces.clear();
	}
	return *this;
}

std::vector<float> Model::generateVertexBufferData(const Vertex vantagePoint, float distanceFromScreen) {
	std::vector<float> bufferData;
	for (TriangleFace face : m_faces) {
		if (face.isVisible(vantagePoint)) {
			for (Vertex vertex : face.vertices) {
				float pX = vertex.x;
				float pY = vertex.y;
				float pZ = vertex.z;
				float eX = vantagePoint.x;
				float eY = vantagePoint.y;
				float eZ = vantagePoint.z;
				float xPrime = ((pX - eX) * distanceFromScreen) / (pZ - eZ);
				float yPrime = ((pY - eY) * distanceFromScreen) / (pZ - eZ);
				float zPrime = 0.0f;
				bufferData.push_back(xPrime);
				bufferData.push_back(yPrime);
				bufferData.push_back(zPrime);
			}
		}
	}
	return bufferData;
}
