#include <string>
#include <vector>
#include <variant>
#include <cassert>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Utils.h"
#include "Pattern/PatternEvaluator.h"
#include "ResultsDatabase/ResultsDatabase.h"
#include "ClauseStrategy/ClauseStrategyContext.h"
#include "ClauseStrategy/RelationStrategy.h"
#include "ClauseStrategy/PatternStrategy.h"
#include "SuchThat/RelationEvaluator.h"


template <class... Ts>
struct Overload : Ts... {
	using Ts::operator()...;
};

template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	pattern = query->patterns;
	declarations = query->declarations;
	target = query->target;
	rdb = ResultsDatabase();

	// Initialize context for strategy pattern
	// auto clauses = prioritizeClauses(query)
	// for clause in clauses:
	// if(!execute(clause)) { return False }

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

	// Return empty list if any of them returns False.
	if (!relClauseResult || !patClauseResult) {
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

	return results;
}


// Relation execute
bool QueryExecutor::relationExecute(Relation relations, ResultsDatabase& rdb) {
	return RelationEvaluator(declarations, relations, rdb, pkb).evaluate();
}

// Pattern execute
bool QueryExecutor::patternExecute(Pattern pattern, ResultsDatabase& rdb) {
	return PatternEvaluator(declarations, pattern, rdb, pkb).evaluate();
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
    ////! Given a single target, if it is a declaration then simply call the rdb api
    ////! If it is attribute reference, then we need to get the declaration first, then get the respective values before returning
    //return std::visit(Overload {
    //        [&](Declaration decl) -> std::unordered_set<std::string> {
    //            return rdb.getResults(decl);
    //        },
    //        [&](AttrReference attrRef) -> std::unordered_set<std::string> {
    //            /**
    //             * TODO: Need to implement this after I understand what is attribute reference
    //            Declaration decl = getDeclarationFromAttrRef(attrRef, declarations);
    //            std::unordered_set<std::string> results = rdb.getSynonymSet(decl);
    //            std::unordered_set<std::string> filteredResults;
    //            for (std::string result : results) {
    //                std::string attrValue = getAttrValueFromAttrRef(attrRef, result, pkb);
    //                filteredResults.insert(attrValue);
    //            }
    //            return filteredResults;
    //             */
    //            return std::unordered_set<std::string>();
    //        }
    //}, target);
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
