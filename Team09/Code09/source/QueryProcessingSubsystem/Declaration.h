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
		Assign,
		Variable,
		Constant,
		Procedure,
		NONE
	};


	DesignEntity TYPE;
	std::string name;

	Declaration(DesignEntity TYPE, std::string name) : TYPE(TYPE), name(name) {};
	Declaration() : TYPE(DesignEntity::NONE), name("") {};

};