#pragma once

#include <vector>
#include <utility>
#include <objects/Model.h>
#include <objects/Vertex.h>

std::vector<float> Model::generateVertexBufferData(const Vertex vantagePoint, float distanceFromScreen) {
	std::vector<float> bufferData;
	for (TriangleFace face : faces) {
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
