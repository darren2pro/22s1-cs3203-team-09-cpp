#pragma once

#include "Strategy.h"
#include "../With/WithEvaluator.h"
#include "../../Clause.h"

class WithStrategy : public Strategy {
public:
    WithStrategy(std::vector<Declaration> declarations, PKBStorage* pkb) :
            Strategy(declarations, pkb) {};

    bool execute(Clause& clause, ResultsDatabase& rdb) const override {
        With* rel = std::get_if<With>(&clause.clause);
        if (rel == nullptr) return false;
        return WithEvaluator(declarations, *rel, rdb, pkb).evaluate();
    }
};
