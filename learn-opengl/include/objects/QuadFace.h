#pragma once

#include "../objects/Vertex.h"

const int NUMBER_OF_VERTICES = 4;

class QuadFace {
private:
	Vertex vertices[NUMBER_OF_VERTICES];

public:
	QuadFace(const Vertex(&vertices)[NUMBER_OF_VERTICES]);

	const Vertex* getVertices() const;
};