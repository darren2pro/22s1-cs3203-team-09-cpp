#pragma once
#include <string>
#include <vector>
#include "Pattern.h"

class Utils {
public:
	static bool isSynonym(std::string argument) {
		return argument.find_first_of("\"") == std::string::npos; // FFO returns npos if " not found. Synonym should not contain " "
	};
	static bool isString(std::string argument) {
		return argument.find_first_of("\"") != std::string::npos; // FFO returns npos if " not found. String should contain " "
	};
	static bool isUnderscore(std::string argument) {
		return argument == "_";
	}
	static bool isStrictExpression(Pattern::Types type) {
		return type == Pattern::Types::AssignStrict;
	}
	static bool isRelaxedExpression(Pattern::Types type) {
		return type == Pattern::Types::AssignRelaxed;
	}
};
