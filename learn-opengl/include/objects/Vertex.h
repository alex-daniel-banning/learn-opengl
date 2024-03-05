#pragma once

class Vertex {
private:
	float x, y, z;

public:
	Vertex();
	Vertex(float x, float y, float z);
	//Vertex operator-(const Vertex& other) const;
	float getX() const;
	float getY() const;
	float getZ() const;
};
