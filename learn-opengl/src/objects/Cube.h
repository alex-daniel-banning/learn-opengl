#pragma once

#include "../objects/QuadFace.h"
#include "../objects/Vertex.h"

class Cube {
private:
	QuadFace faces[6];

public:
	int calculateNumberOfVisibleVertices(Vertex &vantagePoint);
	float* getVisible
};
