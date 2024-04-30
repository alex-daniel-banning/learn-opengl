#pragma once

#include <glad/glad.h>
#include <vector>
#include <utility>
#include <objects/Model.h>
#include <objects/Vertex.h>
#include <misc/Shaders.h>
#include <iostream>

Model::Model(const Model& other) {
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_vantagePoint = Vertex(other.m_vantagePoint);
	m_distanceFromScreen = other.m_distanceFromScreen;
	m_shader = Shader(other.m_shader);
	for (TriangleFace face : other.m_faces) {
		m_faces.push_back(TriangleFace(face));
	}
}

Model::Model(Model&& other) noexcept
	: m_faces(std::move(other.m_faces)), m_vantagePoint(std::move(other.m_vantagePoint)), m_distanceFromScreen(other.m_distanceFromScreen),
		m_VAO(other.m_VAO), m_VBO(other.m_VBO), m_shader(Shader(other.m_shader)) {
	other.m_faces.clear();
}

Model::Model(const std::vector<TriangleFace> faces)
	: m_distanceFromScreen(0) {
	m_distanceFromScreen = 0;
	m_vantagePoint = Vertex(0.0f, 0.0f, 0.0f);
	for (const TriangleFace face : faces) {
		m_faces.push_back(TriangleFace(face));
	}

	m_shader = Shader(Shaders::getModelVertexShader(), Shaders::getModelFragmentShader());

    // configure VAO/VBO for model
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model& Model::operator=(const Model& other) {
	if (this != &other) {
		m_VAO = other.m_VAO;
		m_VBO = other.m_VBO;
		m_distanceFromScreen = other.m_distanceFromScreen;
		m_vantagePoint = Vertex(other.m_vantagePoint);
		m_shader = Shader(other.m_shader);
		m_faces.clear();
		for (const TriangleFace face : other.m_faces) {
			m_faces.push_back(TriangleFace(face));
		}
	}
	return *this;
}

Model& Model::operator=(Model&& other) noexcept {
	if (this != &other) {
		m_VAO = other.m_VAO;
		m_VBO = other.m_VBO;
		m_distanceFromScreen = other.m_distanceFromScreen;
		m_vantagePoint = std::move(other.m_vantagePoint);
		m_shader = std::move(other.m_shader);
		m_faces = std::move(other.m_faces);
		other.m_faces.clear();
	}
	return *this;
}

std::vector<float> Model::generateBufferData() const {
	std::vector<float> bufferData;
	for (TriangleFace face : m_faces) {
		if (face.isVisible(m_vantagePoint)) {
			for (Vertex vertex : face.vertices) {
				float pX = vertex.x;
				float pY = vertex.y;
				float pZ = vertex.z;
				float eX = m_vantagePoint.x;
				float eY = m_vantagePoint.y;
				float eZ = m_vantagePoint.z;
				float xPrime = ((pX - eX) * m_distanceFromScreen) / (pZ - eZ);
				float yPrime = ((pY - eY) * m_distanceFromScreen) / (pZ - eZ);
				float zPrime = 0.0f;
				bufferData.push_back(xPrime);
				bufferData.push_back(yPrime);
				bufferData.push_back(zPrime);
			}
		}
	}
	return bufferData;
}

void Model::setVantagePoint(Vertex vantagePoint) {
	m_vantagePoint = vantagePoint;
}

void Model::setDistanceFromScreen(float distanceFromScreen) {
	m_distanceFromScreen = distanceFromScreen;
}

void Model::setShader(Shader& shader) {
	m_shader = Shader(shader);
}

void Model::render() const {
	// Activate wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	std::vector<float> bufferData = generateBufferData();
	m_shader.use();
    std::cout << glGetError() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindVertexArray(m_VAO);
	glBufferData(
		GL_ARRAY_BUFFER,
		bufferData.size() * sizeof(float),
		bufferData.data(),
		GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, bufferData.size());
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
