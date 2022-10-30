#include "ClausePrioritizer.h"
#include "ClauseFunctions.cpp"

vector<Clause> ClausePrioritizer::getClauses() {
    if (tooFewClauses()) return getClausesFromQuery();
    //! Initialize all clauses to a default starting score and group
    vector<WeightedGroupedClause> weightedGroupedClauses = getInitialWeightedGroupedClauses();
    //! Now we prioritize the clauses
    prioritizeClauses(weightedGroupedClauses);
    // return getClausesFromWeightedGroupedClauses(weightedGroupedClauses);
    return {};
}

bool ClausePrioritizer::tooFewClauses() {
    size_t totalNumOfClauses = query->relations.size() +
                               query->patterns.size() +
                               query->withs.size();
    return totalNumOfClauses < MIN_NUM_OF_CLAUSES_TO_SORT;
}

vector<Clause> ClausePrioritizer::getClausesFromQuery() {
    vector<Clause> clauses;
    for (Relation& rel : query->relations) {
        Clause clause = Clause(rel);
        clauses.push_back(clause);
    }
    for (Pattern& pattern : query->patterns) {
        Clause clause = Clause(pattern);
        clauses.push_back(clause);
    }
    for (With& with : query->withs) {
        Clause clause = Clause(with);
        clauses.push_back(clause);
    }
    return clauses;
}

vector<WeightedGroupedClause> ClausePrioritizer::getInitialWeightedGroupedClauses() {
    vector<WeightedGroupedClause> weightedGroupedClauses;
    for (Relation& rel : query->relations) {
        Clause clause = Clause(rel);
        weightedGroupedClauses.push_back({clause, STARTING_WEIGHT, DEFAULT_GROUP});
    }
    for (Pattern& pattern : query->patterns) {
        Clause clause = Clause(pattern);
        weightedGroupedClauses.push_back({clause, STARTING_WEIGHT, DEFAULT_GROUP});
    }
    for (With& with : query->withs) {
        Clause clause = Clause(with);
        weightedGroupedClauses.push_back({clause, STARTING_WEIGHT, DEFAULT_GROUP});
    }
    return weightedGroupedClauses;
}

void ClausePrioritizer::prioritizeClauses(vector<WeightedGroupedClause>& clauses) {
    //! We apply weightages and penalties on the clauses then sort them

    //! First we apply functions on the clauses to test if they need to have their weights adjusted,
    //! and adjust them if needed
    for (WeightedGroupedClause& clause : clauses) {
        for (WeightFunction weightFunction : weightUpdaters) {
            weightFunction(clause);
        }
    }

}

vector<WeightFunction> ClausePrioritizer::weightUpdaters = {
    weightBooleanClause/*, weightAffectsClause, weightAffectsTClause,
    weightNextTClause, weightPartialPatternMatch, weightCompletePatternMatch*/
};
