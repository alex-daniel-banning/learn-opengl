#pragma once
#include <objects/Model.h>
#include <string>
#include <windows.h>

class ModelFileInputReader {
public:
	static Model readModelFromFile(PWSTR file);

private:
	static std::string trim(const std::string& str);
};
