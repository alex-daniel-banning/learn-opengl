#pragma once

#include <string>
#include <codecvt>
#include <locale>
#include <windows.h>

class StringConverter {
public:
	static std::string convertToString(PWSTR orig);
};
