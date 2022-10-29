#pragma once
#include "With.h"
#include "Pattern.h"
#include "Relation.h"
#include <variant>

class Clause {
private:
    bool isRelationAttr = false;
    bool isPatternAttr = false;
    bool isWithAttr = false;

public:
	std::variant<Relation, Pattern, With> clause;
	
	Clause(Relation relation) : clause(relation), isRelationAttr(true) {};
	Clause(Pattern pattern) : clause(pattern), isPatternAttr(true) {};
	Clause(With with) : clause(with), isWithAttr(true) {};

    bool isRelation() const {
        return isRelationAttr;
    }

    bool isPattern() const {
        return isPatternAttr;
    }

    bool isWith() const {
        return isWithAttr;
    }
};
