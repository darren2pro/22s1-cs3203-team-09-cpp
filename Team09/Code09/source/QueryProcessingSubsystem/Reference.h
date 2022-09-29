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

	Reference(Types TYPE, std::string value) : TYPE(TYPE), value(value), declaration(Declaration()) {};
	Reference(Types TYPE, Declaration declaration) : TYPE(TYPE), value(""), declaration(declaration) {};
	Reference() : TYPE(Types::NONE), value(""), declaration(Declaration()) {};

	bool isSynonym() { TYPE == Types::Synonym; }
	bool isUnderscore() { TYPE == Types::Underscore; }
	bool isString() { TYPE == Types::String; }
	bool isStmtNum() { TYPE == Types::StmtNum; }
};