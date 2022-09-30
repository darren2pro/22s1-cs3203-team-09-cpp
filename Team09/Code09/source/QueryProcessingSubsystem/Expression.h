#pragma once
#include <string>
#include <vector>

class Expression {
public:
	enum Types {
		Underscore,		// '_'
		Strict,			// 'x + 1'
		Relaxed,		// '_"x + 1"_'
		NONE
	};

	Types TYPE;
	std::string value;

	Expression(Types TYPE, std::string value) : TYPE(TYPE), value(value) {};
	Expression() : TYPE(Types::NONE), value("") {};

	bool isUnderscore() { return TYPE == Types::Underscore; }
	bool isStrict() { return TYPE == Types::Strict; }
	bool isRelaxed() { return TYPE == Types::Relaxed; }
};
