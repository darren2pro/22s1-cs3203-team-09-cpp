#include <string>
#include <vector>
#include <variant>
#include <cassert>
#include "QueryExecutor.h"
#include "../Utils.h"
#include "Pattern/PatternEvaluator.h"
#include "ResultsDatabase/ResultsDatabase.h"
#include "ClauseStrategy/ClauseStrategyContext.h"
#include "ClauseStrategy/RelationStrategy.h"
#include "ClauseStrategy/PatternStrategy.h"
#include "SuchThat/RelationEvaluator.h"
#include "SuchThat/NextTRelationEvaluator.h"
#include "SuchThat/AffectsRelationEvaluator.h"
#include "SuchThat/AffectsTRelationEvaluator.h"
#include "With/WithEvaluator.h"
#include "ResultsDatabase/ResultsDatabase.h"


template <class... Ts>
struct Overload : Ts... {
	using Ts::operator()...;
};

template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	pattern = query->patterns;
    with = query->withs;
	declarations = query->declarations;
	target = query->target;
	rdb = ResultsDatabase();

	// Relations clause
    bool relClauseResult = true;

	// Instantiate the ClauseStrategyContext with a concrete strategy first
	ClauseStrategyContext clauseStrategyContext(std::make_unique<RelationStrategy>(declarations, pkb));
    for (Relation& rel : relations) {
        if (!clauseStrategyContext.execute(rel, rdb)) {
            relClauseResult = false;
            break;
        }
    }

	// Patterns clause
    bool patClauseResult = true;
	clauseStrategyContext.setStrategy(std::make_unique<PatternStrategy>(declarations, pkb));
    for (Pattern& pat : pattern) {
        if (!clauseStrategyContext.execute(pat, rdb)) {
            patClauseResult = false;
            break;
        }
    }

	// With clause
    bool withClauseResult = true;
    for (With& wit : with) {
        if (!withExecute(wit, rdb)) {
            withClauseResult = false;
            break;
        }
    }

	// Return empty list if any of them returns False.
	if (!relClauseResult || !patClauseResult || !withClauseResult) {
        // TEMPORARY FIX -> MOVING FORWARD, MUST FORCE EVERYONE TO GO THROUGH getResultsFromRDB
        if (target.isBoolean()) {
            return std::unordered_set<std::string>{"FALSE"};
        }
        else {
			return {};
        }
	}

	// Insert all other variables that have not been inserted.
	for (Declaration decl : declarations) {
		insertSynonymSetIntoRDB(decl, rdb, pkb);
	}

	std::unordered_set<std::string> results = getResultsFromRDB(target, rdb);

    pkb->clearCache();

	return results;
}


// Relation execute
bool QueryExecutor::relationExecute(Relation relations, ResultsDatabase& rdb) {
    Relation::Types type = relations.Type;
    switch (type) {
    case Relation::NextT:
        return NextTRelationEvaluator(declarations, relations, rdb, pkb).evaluate();
    case Relation::Affects:
        NextTRelationEvaluator(declarations, relations, rdb, pkb).computeFully();
        return AffectsRelationEvaluator(declarations, relations, rdb, pkb).evaluate();
    case Relation::AffectsT:
        NextTRelationEvaluator(declarations, relations, rdb, pkb).computeFully();
        AffectsRelationEvaluator(declarations, relations, rdb, pkb).computeFully();
        return AffectsTRelationEvaluator(declarations, relations, rdb, pkb).evaluate();
    default:
        return RelationEvaluator(declarations, relations, rdb, pkb).evaluate();
    }
}

// Pattern execute
bool QueryExecutor::patternExecute(Pattern pattern, ResultsDatabase& rdb) {
	return PatternEvaluator(declarations, pattern, rdb, pkb).evaluate();
}

// With execute
bool QueryExecutor::withExecute(With with, ResultsDatabase& rdb) {
    //WithEvaluator t =  WithEvaluator(declarations, with, rdb, pkb);
    //return t.evaluate();
    return WithEvaluator(declarations, with, rdb, pkb).evaluate();
    //return true;
}

std::unordered_set<std::string> QueryExecutor::getResultsFromRDB(Result result, ResultsDatabase& rdb) {
    if (result.isBoolean()) {
        if (rdb.getBoolean()) {
            return std::unordered_set<std::string>{"TRUE"};
        }
        else {
            return std::unordered_set<std::string>{"FALSE"};
        }
    }
    else if (result.isTuple()) {
        std::vector<std::string> allSynonyms = getSynonyms(result.target);
        std::vector<std::vector<std::string>> allResults = rdb.getMultipleTarget(allSynonyms);
        return combineResults(allResults);
    }
    else {
        assert("False");
    }
}

void QueryExecutor::insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage* pkb) {
	if (rdb.variableIsPresent(decl.name)) return;
	std::unordered_set<std::string> resultsFromPKB = pkb->getEntitySet(decl.Type);
	rdb.insertList(decl.name, resultsFromPKB);
}

std::vector<std::string> QueryExecutor::getSynonyms(std::vector<std::variant<Declaration, AttrReference>>& targets) {
    std::vector<std::string> allSynonyms;
    for ( auto& target : targets) {
        if (auto declPtr = std::get_if<Declaration>(&target)) {
            allSynonyms.push_back(declPtr->name);
        }
        else if (auto attrRefPtr = std::get_if<AttrReference>(&target)) {
            allSynonyms.push_back(attrRefPtr->declaration.name);
        }
    }
    return allSynonyms;
}

std::unordered_set<std::string> QueryExecutor::combineResults(std::vector<std::vector<std::string>> allResults) {
    // Within the string, the values are separated by comma
    std::vector<std::vector<std::string>> finalResults;
    std::unordered_set<std::string> finalFormattedResults;

    // Add the first row of results before starting to iterate.
    for (auto& res : allResults[0]) {
        std::vector<std::string> v;
        v.push_back(res);
        finalResults.push_back(v);
    }

    for (int i = 1; i < allResults.size(); i++) { // for each synonym
		std::vector<std::vector<std::string>> newFinalResults;
		for (int k = 0; k < finalResults.size(); k++) { // for each final "string"

            auto temp = finalResults[k]; // This is the pair that is being modified.

			for (int j = 0; j < allResults[i].size(); j++) { // each synonym's values
                auto constant = temp; // Just a copy
                constant.push_back(allResults[i][j]);
                newFinalResults.push_back(constant);
            }
        }
        finalResults = newFinalResults;
    }

    for (auto& listOfResults : finalResults) {
        finalFormattedResults.insert(formatString(listOfResults));
    }
    return finalFormattedResults;
}

std::string QueryExecutor::formatString(std::vector<std::string> strings) {
    std::string finalString = strings[0];
    for (int i = 1; i < strings.size(); i++) {
        finalString += " " + strings[i];
    }
    return finalString;
}
