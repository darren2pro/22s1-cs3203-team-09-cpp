#pragma once
#include "Strategy.h"
#include "../SuchThat/RelationEvaluator.h"
#include "../../Clause.h"

class RelationStrategy : public Strategy {
public:
	RelationStrategy(std::vector<Declaration> declarations, PKBStorage* pkb) :
	Strategy(declarations, pkb) {};

	bool execute(Clause& clause, ResultsDatabase& rdb) const override {
		Relation* rel = std::get_if<Relation>(&clause.clause);
		return RelationEvaluator(declarations, *rel, rdb, pkb).evaluate();
	}
};
