#pragma once

#include <objects/Vertex.h>
#include <objects/TriangleFace.h>
#include <render/Shader.h>
#include <vector>

class Model {
	
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
	void incrementRotation(float rotation);
	void setZTranslation(float translation);

	void render(const Vertex& vantagePoint, const float& distanceFromScreen) const;

private:
	std::vector<TriangleFace> m_faces;
	Vertex m_vantagePoint;
	float m_distanceFromScreen;
	Shader m_shader;
	unsigned int m_VAO, m_VBO;

	// rotations in radians
	float m_rotation;
	float m_zTranslation;

	std::vector<float> generateBufferData(const Vertex& vantagePoint, const float& distanceFromScreen) const;
};
