#include <objects/Button.h>
#include <objects/Vertex.h>

Button::Button(const Button& other) {
	for (const TriangleFace face : other.faces) {
		faces.push_back(TriangleFace(face));
	}
}

Button::Button(Button&& other) noexcept {
	faces = std::move(other.faces);
	other.faces.clear();
}

Button& Button::operator=(const Button& other) {
	if (this != &other) {
		faces.clear();
		for (const TriangleFace face : other.faces) {
			faces.push_back(TriangleFace(face));
		}
	}
	return *this;
}

Button& Button::operator=(Button&& other) noexcept {
	if (this != &other) {
		faces = std::move(other.faces);
		other.faces.clear();
	}
	return *this;
}

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
