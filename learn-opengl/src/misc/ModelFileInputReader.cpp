#pragma once

#include <exceptions/ModelImportException.h>
#include <misc/ModelFileInputReader.h>
#include <misc/StringConverter.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

Model ModelFileInputReader::readModelFromFile(std::string filepath) {
	validateFileType(filepath);

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

	validateVerticesMetadata(tokens);
	
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
		if (coordTokens.size() != 3) {
			throw ModelImportException("Malformed coordinate found when reading in vertex data.");
		}
		Vertex vertex = Vertex(std::stof(coordTokens[0]), std::stof(coordTokens[1]), std::stof(coordTokens[2]));
		vertices.push_back(vertex);
	}

	/* Handle second section */
	std::string emptyLine;
	std::getline(filestream, emptyLine);
	if (emptyLine != "") {
		throw ModelImportException("There should be an empty line after vertex data and before face metadata.");
	}

	std::string faceHeader;
	std::getline(filestream, faceHeader);
	std::stringstream faceHeadersStream(faceHeader);
	std::string faceHeaderToken;
	std::vector<std::string> faceHeaderTokens;
	while (std::getline(faceHeadersStream, faceHeaderToken, ' ')) {
		faceHeaderTokens.push_back(faceHeaderToken);
	}

	validateFacesMetadata(faceHeaderTokens);
	
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
		validateFace(faceTokens, vertices.size());
		// Create the 3 vertices
		std::vector<Vertex> triangleFaceVertices;
		for (int vertexIndex : faceTokens) {
			Vertex v = Vertex(vertices[vertexIndex]);
			triangleFaceVertices.push_back(v);
		}	
		TriangleFace triangleFace = TriangleFace(triangleFaceVertices);
		triangleFaces.push_back(triangleFace);
	}
	return Model(std::move(triangleFaces));
}

Model ModelFileInputReader::readModelFromFile(PWSTR filepath_pwstr) {
	
	/* Convert to string */
	std::string filepath = StringConverter::convertToString(filepath_pwstr);
	return readModelFromFile(filepath);
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

void ModelFileInputReader::validateFileType(const std::string& str) {
	std::filesystem::path path(str);
	if (path.extension().string() != ".model") {
		throw ModelImportException("You have to select a .model file.");
	}
}

void ModelFileInputReader::validateVerticesMetadata(std::vector<std::string> &tokens) {
	if (tokens.size() != 2 || tokens[0] != "vertices") {
		throw ModelImportException("There was an error when processing vertices metadata.");
	}
}

void ModelFileInputReader::validateFacesMetadata(std::vector<std::string> &tokens) {
	if (tokens.size() != 2 || tokens[0] != "faces") {
		throw ModelImportException("There was an error when processing faces metadata.");
	}
}

void ModelFileInputReader::validateFace( std::vector<int> faceTokens, int verticesCount) {
	for (int vertexIndex : faceTokens) {
		if (vertexIndex >= verticesCount) {
			throw ModelImportException("Encountered a face with more than 3 vertices.");
		}
	}
}
