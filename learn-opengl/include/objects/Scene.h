#pragma once

#include <vector>
#include <memory>
#include <objects/Renderable.h>

class Scene {
private:
	//std::vector<const Renderable*> m_objects;
	std::vector<std::shared_ptr<Renderable>> m_objects;

public:
	void addObject(std::shared_ptr<Renderable> obj) {
		m_objects.push_back(obj);
	}

	void renderScene() const {
		for (const std::shared_ptr<Renderable>& obj : m_objects) {
			obj->render();
		}
	}
};
