#pragma once
#include <string>
#include <vector>

class Declaration {
public:
	enum DesignEntity {
		Statement,
		Read,
		Print,
		While,
		If,
		Assignment,
		Variable,
		Constant,
		Procedure,
		Call,
		NONE
	};

	// Add a TYPE enum class for underscore/synonym/simple in the future after MS1.

	DesignEntity TYPE;
	std::string name;

	Declaration(DesignEntity TYPE, std::string name) : TYPE(TYPE), name(name) {};
	Declaration() : TYPE(DesignEntity::NONE), name("") {};

};