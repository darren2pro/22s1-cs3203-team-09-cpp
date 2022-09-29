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

	bool isUnderscore() { TYPE == Types::Underscore; }
	bool isStrict() { TYPE == Types::Strict; }
	bool isRelaxed() { TYPE == Types::Relaxed; }
};
