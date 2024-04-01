#pragma once
#include <misc/ModelFileInputReader.h>
#include <misc/StringConverter.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

Model ModelFileInputReader::readModelFromFile(PWSTR filepath_pwstr) {
	
	/* Convert to string */
	std::string filepath = StringConverter::convertPWSTRToString(filepath_pwstr);

	/* Handle first section */
	std::ifstream filestream;
	filestream.open(filepath);
	std::string firstLine;
	std::getline(filestream, firstLine);

	std::string verticesHeaders;
	std::stringstream ss(firstLine);
	std::vector<std::string> tokens;
	while (std::getline(ss, verticesHeaders, ' ')) {
		tokens.push_back(verticesHeaders);
	}

	// todo - more comprehensive validation
	assert(tokens.size() == 2);
	assert(tokens[0] == "vertices");
	
	int numUniqueVertices = std::stoi(tokens[1]);
	std::vector<Vertex> vertices;
	for (int i = 0; i < numUniqueVertices; i++) {
		// get the line for the vertex
		std::string vertexLine;
		std::getline(filestream, vertexLine);

		// add vertex to list of vertices
		std::string coordStr;
		std::stringstream vertexLineStream(vertexLine);
		std::vector<std::string> coordTokens;
		while (std::getline(vertexLineStream, coordStr, ',')) {
			coordTokens.push_back(trim(coordStr));
		}
		assert(coordTokens.size() == 3);
		Vertex vertex = Vertex(std::stof(coordTokens[0]), std::stof(coordTokens[1]), std::stof(coordTokens[2]));
		vertices.push_back(vertex);
	}

	/* Handle second section */
	std::string emptyLine;
	std::getline(filestream, emptyLine);
	assert(emptyLine == "");

	std::string faceHeader;
	std::getline(filestream, faceHeader);
	std::stringstream faceHeadersStream(faceHeader);
	std::string faceHeaderToken;
	std::vector<std::string> faceHeaderTokens;
	while (std::getline(faceHeadersStream, faceHeaderToken, ' ')) {
		faceHeaderTokens.push_back(faceHeaderToken);
	}
	assert(faceHeaderTokens[0] == "faces");
	int numberOfFaces = stoi(faceHeaderTokens[1]);

	std::vector<TriangleFace> triangleFaces;
	for (int i = 0; i < numberOfFaces; i++) {
		std::string faceline;
		std::getline(filestream, faceline);
		std::stringstream faceStream(faceline);
		std::string faceToken;
		std::vector<int> faceTokens;
		while (std::getline(faceStream, faceToken, ',')) {
			faceTokens.push_back(std::stoi(faceToken));
		}
		// Create triangle face
		assert(faceTokens.size() == 3);
		// Create the 3 vertices
		std::vector<Vertex> triangleFaceVertices(3);
		for (int vertexIndex : faceTokens) {
			assert(false); // reminder for todo below
			Vertex v = Vertex(vertices[vertexIndex]); // todo copy constructor
			triangleFaceVertices.push_back(v);
		}	
		TriangleFace triangleFace = TriangleFace(triangleFaceVertices);
		triangleFaces.push_back(triangleFace);
	}
	return Model(std::move(triangleFaces));
}

std::string ModelFileInputReader::trim(const std::string& str) {
	if (str == "") {
		return str;
	}
	size_t first = str.find_first_not_of(" \n\r\t");
	if (std::string::npos == first) {
		return str;
	}
	size_t last = str.find_last_not_of(" \n\r\t");
	return str.substr(first, (last - first + 1));
}
