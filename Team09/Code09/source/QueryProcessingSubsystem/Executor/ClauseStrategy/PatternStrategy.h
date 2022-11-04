#pragma once
#include "Strategy.h"
#include "../Pattern/PatternEvaluator.h"

class PatternStrategy : public Strategy {
public:
	PatternStrategy(std::vector<Declaration> declarations, PKBStorage* pkb) :
	Strategy(declarations, pkb) {};

	bool execute(Clause& clause, ResultsDatabase& rdb) const override {
		Pattern* pat = std::get_if<Pattern>(&clause.clause);
		return PatternEvaluator(declarations, *pat, rdb, pkb).evaluate();
	}
};
