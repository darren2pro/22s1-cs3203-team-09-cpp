#pragma once
#include <string>
#include <vector>
#include "Pattern.h"

class Utils {
public:
    /**
     * Checks whether the provided arg is a valid synonym by referencing the declarations vector from the query adt.
     * @param arg string arg to check whether it is a valid synonym
     * @param declarations vector of declarations from the query adt
     * @return true if arg is a valid synonym, false otherwise
     */
	static bool isSynonym(std::string arg, std::vector<Declaration> declarations) {
		for (Declaration decl : declarations) {
			if (decl.name == arg) {
				return true;
			}
		}
		return false;
	};

    /**
     * Gets the declaration object for this synonym
     */
	static Declaration getSynonym(std::string arg, std::vector<Declaration> declarations) {
		for (Declaration decl : declarations) {
			if (decl.name == arg) {
				return decl;
			}
		}
		return Declaration(); // Should never be reached
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
