#pragma once
#include "With.h"
#include "Pattern.h"
#include "Relation.h"
#include <variant>

class Clause {
public:
	std::variant<Relation, Pattern, With> clause;
	bool isRelation = false;
	bool isPattern = false;
	bool isWith = false;
	
	Clause(Relation relation) : clause(relation), isRelation(true) {};
	Clause(Pattern pattern) : clause(pattern), isPattern(true) {};
	Clause(With with) : clause(with), isWith(true) {};
};
