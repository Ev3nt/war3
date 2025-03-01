#include "Utils.h"

#include <Windows.h>

std::string Utils::getEnv(const std::string& key) {
	std::string value;
	DWORD lenght;

	while ((lenght = GetEnvironmentVariable(key.data(), value.data(), value.size() + 1)) != value.size()) {
		value.resize(lenght - 1);
	}

	return value;
}

std::vector<std::string> Utils::slice(const std::string& string, char delimiter) {
	std::vector<std::string> result;
	size_t beg = 0;

	for (size_t end = 0; end != std::string::npos && !string.empty();) {
		end = string.find(delimiter, beg);
		result.push_back(string.substr(beg, end - beg));
		beg = end + 1;
	}

	return result;
}

std::string Utils::trim(const std::string& string) {
	size_t beg = 0;
	size_t end = string.size();

	for (; beg < end && std::isspace(string[beg]); beg++);
	for (; end > beg && std::isspace(string[--end]););

	return string.substr(beg, end - beg + 1);
}