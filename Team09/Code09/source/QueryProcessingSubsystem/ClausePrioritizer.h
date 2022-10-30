#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <variant>
#include <functional>
#include "Executor/Pattern/PatternEvaluator.h"
#include "Executor/SuchThat/RelationEvaluator.h"
#include "Executor/With/WithEvaluator.h"
#include "Query.h"
#include "Clause.h"

using namespace std;

struct WeightedGroupedClause {
    //! The clause class enclosed in this struct. Refer to Clause.h
    Clause clause;
    //! Lower the score means higher the priority, can be negative. The weight within each group
    int weight;
    //! The same group id means that the clause is grouped together
    unsigned int groupId;

    //! Sort by groupId, then weight
    bool operator<(const WeightedGroupedClause& other) const {
        return std::tie(groupId, weight) < std::tie(other.groupId, other.weight);
    }

    friend std::ostream& operator<<(std::ostream& os, WeightedGroupedClause const& wgc) {
        os << "Group: " << wgc.groupId << ", Weight: " << wgc.weight;
        return os;
    }
};

//! For application of clause weights based on their type
using WeightFunction = function<void(WeightedGroupedClause&)>;
using SuchThatMatcher = function<bool(Relation)>;
using PatternMatcher = function<bool(Pattern)>;
using WithMatcher = function<bool(With)>;

class ClausePrioritizer {
private:
    static const int MIN_NUM_OF_CLAUSES_TO_SORT = 2;
    static const int STARTING_WEIGHT = 100;
    static const int DEFAULT_GROUP = 0;

    /**
     * This is a list of functions that test if a clause is of a certain type and
     * then applies a function to update the weights inside that clause if it
     * matches
     */
    static vector<WeightFunction> weightUpdaters;

    Query* query;

    //! Too little clauses means that there's little to no gain of sorting them
    bool tooFewClauses();

    //! Simply takes all the clauses from the query and puts it into a vector of clauses
    vector<Clause> getClausesFromQuery();

    //! Initializes all intermediate clause objects with a starting weight
    vector<WeightedGroupedClause> getInitialWeightedGroupedClauses();

    //! Calculates weights and groupings for clauses - modifies input parameter
    void prioritizeClauses(vector<WeightedGroupedClause>& clauses);

    //! Retrieves the clause objects from the weighted grouped clauses
    vector<Clause> getClausesFromWeightedGroupedClauses(const vector<WeightedGroupedClause>& clauses);

public:
    explicit ClausePrioritizer(Query* query) : query(query) {};

    vector<Clause> getClauses(bool performOptimized);
};
