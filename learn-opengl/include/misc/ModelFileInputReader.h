#pragma once
#include <objects/Model.h>
#include <string>
#include <windows.h>

class ModelFileInputReader {
public:
	static Model readModelFromFile(PWSTR file);
	static Model readModelFromFile(std::string filepath);

private:
	static std::string trim(const std::string& str);
	static void validateFileType(const std::string& str);
	static void validateVerticesMetadata(std::vector<std::string> &tokens);
	static void validateFacesMetadata(std::vector<std::string> &tokens);
	static void validateFace(std::vector<int> faceTokens, int verticesCount);
};
