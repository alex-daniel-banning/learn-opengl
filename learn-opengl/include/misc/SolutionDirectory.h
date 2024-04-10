#pragma once
#include <string>

class SolutionDirectory {
public:
	static const std::string directory;
};

#ifdef _SOLUTIONDIR
	const std::string SolutionDirectory::directory = _SOLUTIONDIR;
#else
	const std::string SolutionDirectory::directory = "";
#endif
