#pragma once
#include <string>
#include "Expression.h"
#include "Reference.h"
#include "Declaration.h"

class Pattern {
public:
	enum Types {
		NONE,
		Assign,
		If,
		While
	};

	Types TYPE = Types::Assign;		// change in sprint 4
	Declaration synonym;
	Reference LEFT_ARG;
	Expression RIGHT_ARG;

	Pattern(Declaration synonym, Reference LEFT_ARG, Expression RIGHT_ARG) : synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Pattern(Types TYPE, Declaration synonym, Reference LEFT_ARG, Expression RIGHT_ARG) : TYPE(TYPE), synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Pattern() : TYPE(Types::NONE), synonym(Declaration()), LEFT_ARG(Reference()), RIGHT_ARG(Expression()) {};
};
