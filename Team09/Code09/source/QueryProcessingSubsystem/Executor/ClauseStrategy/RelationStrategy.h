#pragma once
#include "../SuchThat/NextTRelationEvaluator.h"
#include "../SuchThat/AffectsTRelationEvaluator.h"
#include "../SuchThat/AffectsRelationEvaluator.h"
#include "../QueryExecutor.h"
#include "Strategy.h"
#include "../SuchThat/RelationEvaluator.h"
#include "../../Clause.h"

class RelationStrategy : public Strategy {
public:
	RelationStrategy(std::vector<Declaration> declarations, PKBStorage* pkb) :
	Strategy(declarations, pkb) {};

	bool execute(Clause& clause, ResultsDatabase& rdb) const override {
		Relation* rel = std::get_if<Relation>(&clause.clause);
		if (rel == nullptr) return false;

        Relation::Types type = rel->Type;
        switch (type) {
        case Relation::NextT:
            return NextTRelationEvaluator(declarations, *rel, rdb, pkb).evaluate();
        case Relation::Affects:
            NextTRelationEvaluator(declarations, *rel, rdb, pkb).computeFully();
            return AffectsRelationEvaluator(declarations, *rel, rdb, pkb).evaluate();
        case Relation::AffectsT:
            NextTRelationEvaluator(declarations, *rel, rdb, pkb).computeFully();
            AffectsRelationEvaluator(declarations, *rel, rdb, pkb).computeFully();
            return AffectsTRelationEvaluator(declarations, *rel, rdb, pkb).evaluate();
        default:
            return RelationEvaluator(declarations, *rel, rdb, pkb).evaluate();
        }
	}
};
