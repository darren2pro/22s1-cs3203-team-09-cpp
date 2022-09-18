#pragma once
#include <string>

class Pattern {
public:
	enum Types {
		Assign,
		With
	};
	std::string synonym;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

	Types TYPE = Types::Assign;

	Pattern(std::string synonym, std::string LEFT_ARG, std::string RIGHT_ARG) : synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Pattern() : synonym(""), LEFT_ARG(""), RIGHT_ARG("") {};
};
