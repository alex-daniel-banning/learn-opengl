#include "../objects/QuadFace.h"

QuadFace::QuadFace(const Vertex(&vertices)[NUMBER_OF_VERTICES]) {
	for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
		this->vertices[i] = vertices[i];
	}
}

const Vertex* QuadFace::getVertices() const {
	return vertices;
}
