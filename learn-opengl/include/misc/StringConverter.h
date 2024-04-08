#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <windows.h>

class StringConverter {
public:
	static std::string convertToString(PWSTR orig);
	static std::unique_ptr<wchar_t[]> convertToWideChar(const char* orig);
};
