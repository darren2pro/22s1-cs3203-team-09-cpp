#pragma once
#include <string>

class Pattern {
public:
	enum Types {		// is this still necessary if sysnoyn is a declaration???
		NONE,
		Assign,
		If,
		While
	};

	std::string synonym;		// Declaration
	std::string LEFT_ARG;		// Reference type
	std::string RIGHT_ARG;		// Expression type

	Types TYPE = Types::Assign;

	Pattern(std::string synonym, std::string LEFT_ARG, std::string RIGHT_ARG) : synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Pattern() : TYPE(Types::NONE), synonym(""), LEFT_ARG(""), RIGHT_ARG("") {};
};
