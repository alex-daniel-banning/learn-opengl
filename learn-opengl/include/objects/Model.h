#pragma once

#include <objects/Vertex.h>
#include <objects/TriangleFace.h>
#include <objects/Renderable.h>
#include <render/Shader.h>
#include <vector>

class Model : public Renderable {
	
public:
	Model() = default;
	~Model() = default;
	Model(const Model& other);
	Model(Model&& other) noexcept;
	Model(const std::vector<TriangleFace> faces);
	Model& operator=(const Model& other);
	Model& operator=(Model&& other) noexcept;

	void setVantagePoint(Vertex vantagePoint);
	void setDistanceFromScreen(float distanceFromScreen);
	void setShader(Shader& shader);

	void render() const;

private:
	std::vector<TriangleFace> m_faces;
	Vertex m_vantagePoint;
	float m_distanceFromScreen;
	Shader m_shader;
	unsigned int m_VAO, m_VBO;

	std::vector<float> generateBufferData() const;
};
