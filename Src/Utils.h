#pragma once

#include <string>
#include <vector>

namespace Utils {
	std::string getEnv(const std::string& key);
	std::vector<std::string> slice(const std::string& string, char delimiter);
	std::string trim(const std::string& string);
}