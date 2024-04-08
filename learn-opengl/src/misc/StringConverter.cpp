#pragma once

#include <memory>
#include <string>
#include <misc/StringConverter.h>

std::string StringConverter::convertToString(PWSTR orig) {
	size_t origsize = std::wcslen(orig) + 1;
	size_t convertedChars = 0;

	const size_t newsize = origsize * 2;
	char* newstring = new char[newsize];

	wcstombs_s(&convertedChars, newstring, newsize, orig, _TRUNCATE);

	std::string returnString(newstring);
	return returnString;
}

std::unique_ptr<wchar_t[]> StringConverter::convertToWideChar(const char* orig) {
	size_t newsize = strlen(orig) + 1;
	std::unique_ptr<wchar_t[]> wcstring = std::make_unique<wchar_t[]>(newsize);

	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring.get(), newsize, orig, _TRUNCATE);
	return wcstring;
}
