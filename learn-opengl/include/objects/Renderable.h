#pragma once

class Renderable {
public:
	virtual void render() const = 0;
	virtual ~Renderable() {}
};
