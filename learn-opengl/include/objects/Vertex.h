#pragma once

class Vertex {
public:
	float x, y, z;

	Vertex() : x(0.0f), y(0.0f), z(0.0f) {};
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {};
};
