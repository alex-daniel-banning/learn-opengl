#pragma once
#include <exception>
#include <string>

class ModelImportException : public std::exception {
public:
	ModelImportException(const std::string& msg) : message(msg) {}
	const char* what() const noexcept override {
		return message.c_str();
	}

private:
	std::string message;
};
