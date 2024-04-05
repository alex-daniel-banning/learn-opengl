#pragma once

#include <misc/StringConverter.h>
#include <string>

std::string StringConverter::convertToString(PWSTR orig) {
	size_t origsize = std::wcslen(orig) + 1;
	size_t convertedChars = 0;

	const size_t newsize = origsize * 2;
	char* newstring = new char[newsize];

	wcstombs_s(&convertedChars, newstring, newsize, orig, _TRUNCATE);

	std::string returnString(newstring);
	return returnString;
}