#pragma once
#include <string>

class Pattern {
public:
	enum Types {
		None,
		Assign,
		With
	};
	std::string synonym;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

	Types TYPE = Types::Assign;

	Pattern(std::string synonym, std::string LEFT_ARG, std::string RIGHT_ARG) : synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Pattern() : TYPE(Types::None), synonym(""), LEFT_ARG(""), RIGHT_ARG("") {};
};
