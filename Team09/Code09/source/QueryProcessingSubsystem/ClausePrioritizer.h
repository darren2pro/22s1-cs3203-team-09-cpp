#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <variant>
#include "Executor/Pattern/PatternEvaluator.h"
#include "Executor/SuchThat/RelationEvaluator.h"
#include "Executor/With/WithEvaluator.h"

using namespace std;

// TODO: To be removed in the future when the proper Clause implementation is done
using Clause = variant<PatternEvaluator, RelationEvaluator, WithEvaluator>;

struct WeightedGroupedEvaluator {
    Clause clause;
    //! Lower the score means higher the priority, can be negative. The weight within each group
    int weight;
    //! The same group id means that the clause is grouped together
    unsigned int groupId;

    //! Sort by groupId, then weight
    bool operator<(const WeightedGroupedEvaluator& other) const {

    }
};

class ClausePrioritizer {

};
