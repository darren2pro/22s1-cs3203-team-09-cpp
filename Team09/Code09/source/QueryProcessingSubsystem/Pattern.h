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

	Types TYPE;
	Declaration synonym;
	Reference LEFT_ARG;
	Expression RIGHT_ARG;	// only has value if type is Assign, otherwise will be an empty Expression object

	Pattern(Types TYPE, Declaration synonym, Reference LEFT_ARG) : TYPE(TYPE), synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(Expression()) {};	// for If and While cases
	Pattern(Types TYPE, Declaration synonym, Reference LEFT_ARG, Expression RIGHT_ARG) : TYPE(TYPE), synonym(synonym), LEFT_ARG(LEFT_ARG), RIGHT_ARG(RIGHT_ARG) {};
	Pattern() : TYPE(Types::NONE), synonym(Declaration()), LEFT_ARG(Reference()), RIGHT_ARG(Expression()) {};

	bool operator==(const Pattern& p) const {
		return TYPE == p.TYPE && synonym == p.synonym && LEFT_ARG == p.LEFT_ARG && RIGHT_ARG == p.RIGHT_ARG;
	}
};
