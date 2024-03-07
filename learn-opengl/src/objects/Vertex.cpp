#include <objects/Vertex.h>

Vertex::Vertex() : x(0.0f), y(0.0f), z(0.0f) {}

Vertex::Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

//Vertex Vertex::operator-(const Vertex& other) const {
//	return Vertex(x - other.getX(), y - other.getY(), z - other.getZ());
//}

float Vertex::getX() const {
	return x;
}

float Vertex::getY() const {
	return y;
}

float Vertex::getZ() const {
	return z;
}

