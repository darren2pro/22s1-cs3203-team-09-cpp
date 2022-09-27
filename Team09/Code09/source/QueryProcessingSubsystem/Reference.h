#pragma once
#include <string>
#include <vector>
#include "Declaration.h"

class Reference {
public:
	enum Types {
		Underscore,
		Synonym,
		String,		// "x"
		StmtNum,
		NONE
	};

	Types TYPE;
	std::string value;
	Declaration declaration;		// this attribute will only have a value if the Reference object is of Type Synonym; 
									// Otherwirse it'll be an empty Declaration

	bool isSynonym();
	bool isUnderscore();
	bool isString();
	bool isStmtNum();
};