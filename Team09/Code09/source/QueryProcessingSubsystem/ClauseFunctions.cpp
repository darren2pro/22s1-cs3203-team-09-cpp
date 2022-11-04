#include <functional>
#include "ClausePrioritizer.h"

using namespace std;
using namespace placeholders;

template<class... Ts>
struct Overload : Ts ... {
    using Ts::operator()...;
};

template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

//! Settings values
//! Generic penalty/reward numbers
constexpr int EXECUTE_ME_FIRST_REWARD = -10000;
constexpr int SET_LOOKUP_PENALTY = 5;
constexpr int EXECUTE_ME_LAST_PENALTY = 10000;
constexpr int NO_DELTA = 0;

//! Pattern penalty for any string matching
constexpr int PARTIAL_PATTERN_MATCH_PENALTY = 600;
constexpr int COMPLETE_PATTERN_MATCH_PENALTY = 300;

//! Penalties for query that are computed on-the-fly
constexpr int ON_THE_FLY_PENALTY = EXECUTE_ME_LAST_PENALTY;
constexpr int AFFECTS_T_PENALTY = 1000;
constexpr int AFFECTS_PENALTY = 500;
constexpr int NEXT_T_PENALTY = 300;

//! This function will be used by all the delta functions ------------------------------------------
void addWeightToClausesConditionally(WeightedGroupedClause& weightedGroupedClause,
                                     int suchThatWeightDelta,
                                     int patternWeightDelta,
                                     int withWeightDelta,
                                     SuchThatMatcher suchThatMatcher,
                                     PatternMatcher patternMatcher,
                                     WithMatcher withMatcher) {
    variant<Relation, Pattern, With> clause = weightedGroupedClause.clause.clause;
    visit(Overload{
            [&](Relation suchThat) {
                if (suchThatMatcher(suchThat)) {
                    weightedGroupedClause.weight += suchThatWeightDelta;
                }
            },
            [&](Pattern pattern) {
                if (patternMatcher(pattern)) {
                    weightedGroupedClause.weight += patternWeightDelta;
                }
            },
            [&](With with) {
                if (withMatcher(with)) {
                    weightedGroupedClause.weight += withWeightDelta;
                }
            },
    }, clause);
}

//! All the matchers ------------------------------------------------------------
//! False matchers for all the three types of clauses
SuchThatMatcher falseSuchThatMatcher = [](Relation) { return false; };
PatternMatcher falsePatternMatcher = [](Pattern) { return false; };
WithMatcher falseWithMatcher = [](With) { return false; };

//! Matching basic clauses. Basic clauses are when they are cheap to execute
SuchThatMatcher suchThatBasicMatcher = [](Relation r) {
    //! E.g. Follows(1, 2)
    //! Basic here means either string or statement number
    const bool leftIsBasic = r.LEFT_ARG.isString() || r.LEFT_ARG.isStmtNum();
    const bool rightIsBasic = r.RIGHT_ARG.isString() || r.RIGHT_ARG.isStmtNum();
    return leftIsBasic && rightIsBasic;
};

PatternMatcher patternBasicMatcher = [](Pattern p) {
    //! E.g. pattern a("x", _) or ifs("x", _, _) or w("x", _)
    //! Basic here means either string or statement number
    const bool firstArgIsBasic = p.LEFT_ARG.isString() || p.LEFT_ARG.isStmtNum();
    const bool rightIsUnderscore = p.RIGHT_ARG.isUnderscore();
    return firstArgIsBasic && rightIsUnderscore;
};

WithMatcher withBasicMatcher = [](With w) {
    //! E.g. with 2 = 3 or with "x" = "y"
    //! Basic here means either string or statement number
    const bool ref1IsQuoteIdent = w.ref1.isIdent();
    const bool ref2IsQuoteIdent = w.ref2.isIdent();

    const bool ref1IsInt = w.ref1.isInteger();
    const bool ref2IsInt = w.ref2.isInteger();

    return (ref1IsQuoteIdent && ref2IsQuoteIdent) || (ref1IsInt && ref2IsInt);
};

//! isAffectsClause matcher. Checks that a relation is an Affects type
SuchThatMatcher suchThatAffectsMatcher = [](Relation r) {
    return r.Type == Relation::getType("Affects");
};

//! isAffectsTClause matcher. Checks that a relation is an AffectsT type
SuchThatMatcher suchThatAffectsTMatcher = [](Relation r) {
    return r.Type == Relation::getType("Affects*");
};

//! isNextTClause matcher. Checks that a relation is an NextT type
SuchThatMatcher suchThatNextTMatcher = [](Relation r) {
    return r.Type == Relation::getType("Next*");
};

//! Partial pattern matcher. Checks that a pattern is a partial pattern
PatternMatcher patternPartialMatcher = [](Pattern p) {
    //! E.g. pattern a("x", _"y"_)
    //! Partial here means that the second argument is relaxed. Only Assign pattern can possibly be partial.
    //! Ifs and Whiles are always strict.
    if (p.Type == Pattern::Types::Assign) {
        const bool secondArgIsRelaxed = p.RIGHT_ARG.TYPE == Expression::Types::Relaxed;
        return secondArgIsRelaxed;
    } else {
        return false;
    }
};

//! Complete pattern matcher. Checks that a pattern is a strict pattern
PatternMatcher patternCompleteMatcher = [](Pattern p) {
    //! E.g. pattern a("x", "x * y")
    //! Complete here means that the second argument is strict. Only Assign pattern can possibly be strict.
    //! Ifs and Whiles are always strict.
    if (p.Type == Pattern::Types::Assign) {
        const bool secondArgIsStrict = p.RIGHT_ARG.TYPE == Expression::Types::Strict;
        return secondArgIsStrict;
    } else {
        return false;
    }
};

//! WeightFunctions -------------------------------------------------------------
//! _1 is a placeholder for the arguments to be passed in from the resulting function object returned from the bind.

//! This function prioritizes clauses that evaluates to boolean significance,
//! e.g. with 2 = 3, pattern a ("x", _), ifs ("x", _, _), w ("x", _), Follows (1, 2)
WeightFunction
        weightBooleanClause = bind(addWeightToClausesConditionally, _1,
        /* suchThatWeightDelta */ EXECUTE_ME_FIRST_REWARD + SET_LOOKUP_PENALTY,
        /* patternWeightDelta */ EXECUTE_ME_FIRST_REWARD + SET_LOOKUP_PENALTY,
        /* withWeightDelta */ EXECUTE_ME_FIRST_REWARD,
                                   suchThatBasicMatcher,
                                   patternBasicMatcher,
                                   withBasicMatcher);

//! Sends Affects clauses to the back of the vector to be executed last with AFFECTS_PENALTY
WeightFunction weightAffectsClause = bind(addWeightToClausesConditionally, _1,
        /* suchThatWeightDelta */ ON_THE_FLY_PENALTY + AFFECTS_PENALTY,
        /* patternWeightDelta */ NO_DELTA,
        /* withWeightDelta */ NO_DELTA,
                                          suchThatAffectsMatcher,
                                          falsePatternMatcher,
                                          falseWithMatcher);

//! Sends AffectsT clauses to the back of the vector to be executed last with AFFECTS_T_PENALTY
WeightFunction weightAffectsTClause = bind(addWeightToClausesConditionally, _1,
        /* suchThatWeightDelta */ ON_THE_FLY_PENALTY + AFFECTS_T_PENALTY,
        /* patternWeightDelta */ NO_DELTA,
        /* withWeightDelta */ NO_DELTA,
                                           suchThatAffectsTMatcher,
                                           falsePatternMatcher,
                                           falseWithMatcher);

//! Sends NextT clauses to the back/almost at the back of the vector with NEXT_T_PENALTY
WeightFunction weightNextTClause = bind(addWeightToClausesConditionally, _1,
        /* suchThatWeightDelta */ ON_THE_FLY_PENALTY + NEXT_T_PENALTY,
        /* patternWeightDelta */ NO_DELTA,
        /* withWeightDelta */ NO_DELTA,
                                        suchThatNextTMatcher,
                                        falsePatternMatcher,
                                        falseWithMatcher);

WeightFunction weightPartialPatternMatch = bind(addWeightToClausesConditionally, _1,
        /* suchThatWeightDelta */ NO_DELTA,
        /* patternWeightDelta */ PARTIAL_PATTERN_MATCH_PENALTY,
        /* withWeightDelta */ NO_DELTA,
                                                falseSuchThatMatcher,
                                                patternPartialMatcher,
                                                falseWithMatcher);

WeightFunction weightCompletePatternMatch = bind(addWeightToClausesConditionally, _1,
        /* suchThatWeightDelta */ NO_DELTA,
        /* patternWeightDelta */ COMPLETE_PATTERN_MATCH_PENALTY,
        /* withWeightDelta */ NO_DELTA,
                                                 falseSuchThatMatcher,
                                                 patternCompleteMatcher,
                                                 falseWithMatcher);
