#pragma once
#include <string>
#include <vector>
#include <regex>
#include "Declaration.h"
#include "Validator/SyntaxException.h"

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

	Reference(std::string value) {
		if (value == "_") {
			this->TYPE = Types::Underscore;
			this->declaration = Declaration();
		} else if (std::regex_match(value, std::regex("\"(.*)\""))) {
			this->TYPE = Types::String;
			value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
			this->declaration = Declaration();
		} else if (std::regex_match(value, std::regex("[0-9]+"))) {
			this->TYPE = Types::StmtNum;
			this->declaration = Declaration();
		} else {
			throw SyntaxError("invalid reference value");
		}
		this->value = value;
	}

	Reference(Declaration declaration) : TYPE(Types::Synonym), value(""), declaration(declaration) {};

	Reference() : TYPE(Types::NONE), value(""), declaration(Declaration()) {};

	bool isSynonym() { return TYPE == Types::Synonym; }
	bool isUnderscore() { return TYPE == Types::Underscore; }
	bool isString() { return TYPE == Types::String; }
	bool isStmtNum() { return TYPE == Types::StmtNum; }
};