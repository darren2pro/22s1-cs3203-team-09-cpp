#pragma once
#include <string>
#include <vector>

class Utils {
public:
	static bool isSynonym(std::string argument) {
		return argument.find_first_of("\"") == std::string::npos; // FFO returns npos if " not found. Synonym should not contain " "
	};
	static bool isString(std::string argument) {
		return argument.find_first_of("\"") != std::string::npos; // FFO returns npos if " not found. String should contain " "
	};
};
