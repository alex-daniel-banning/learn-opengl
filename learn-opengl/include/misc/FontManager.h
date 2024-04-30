#pragma once

#include <string>
#include <misc/SolutionDirectory.h>

class FontManager {
public:
	static std::string getFontPath() {
		return SolutionDirectory::directory + "/resources/fonts/8514oem.ttf";
	}
};
