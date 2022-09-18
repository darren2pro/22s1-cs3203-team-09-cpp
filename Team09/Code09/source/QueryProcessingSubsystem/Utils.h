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
	static bool isStrictExpression(std::string expr) {
		return expr.find_first_of("_") == std::string::npos;
	}
	static bool isRelaxedExpression(std::string expr) {
		return expr.find_first_of("_") != std::string::npos;
	}

	// NEEDS CHANGING
	//static bool isList(Pattern::Types type) {
	//	return type == Pattern::Types::AssignRelaxed;
	//}
	//static bool isListPair(Pattern::Types type) {
	//	return type == Pattern::Types::AssignRelaxed;
	//}
};
