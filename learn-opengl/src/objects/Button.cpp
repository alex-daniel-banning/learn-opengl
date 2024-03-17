#include <objects/Button.h>
#include <objects/Vertex.h>

Button::Button(std::vector<Vertex> vertices, std::vector<int> vertexIndices) {
	for (int i = 0; i < vertexIndices.size(); i += 3) {
		Vertex v1 = vertices[vertexIndices[i]];
		Vertex v2 = vertices[vertexIndices[i + 1]];
		Vertex v3 = vertices[vertexIndices[i + 2]];
		TriangleFace triangleFace = TriangleFace({ v1, v2, v3 });
		faces.push_back(triangleFace);
	}
}

std::vector<float> Button::generateVertexBufferData() {
	std::vector<float> bufferData;
	for (TriangleFace triangleFace : faces) {
		for (Vertex vertex : triangleFace.vertices) {
			bufferData.push_back(vertex.x);
			bufferData.push_back(vertex.y);
			bufferData.push_back(vertex.z);
		}
	}
	return bufferData;
}
