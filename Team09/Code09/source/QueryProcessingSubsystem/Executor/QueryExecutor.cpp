#include <string>
#include <vector>
#include <variant>
#include <cassert>
#include "QueryExecutor.h"
#include "../Utils.h"
#include "ResultsDatabase/ResultsDatabase.h"
#include "ClauseStrategy/ClauseStrategyContext.h"
#include "ClauseStrategy/RelationStrategy.h"
#include "ClauseStrategy/PatternStrategy.h"
#include "ClauseStrategy/WithStrategy.h"
#include "../ClausePrioritizer.h"

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query, bool performOptimized) {
    relations = query->relations;
	pattern = query->patterns;
    with = query->withs;
	declarations = query->declarations;
	target = query->target;
	rdb = ResultsDatabase();

	// TODO: For Darren.
    std::shared_ptr<WithStrategy> withStrat = std::make_shared<WithStrategy>(declarations, pkb);
    std::shared_ptr<PatternStrategy> patternStrat = std::make_shared<PatternStrategy>(declarations, pkb);
    std::shared_ptr<RelationStrategy> relationStrat = std::make_shared<RelationStrategy>(declarations, pkb);
    ClauseStrategyContext clauseStrategyContext(withStrat);

    std::vector<Clause> clauses = ClausePrioritizer(query).getClauses(performOptimized);

    for (auto& clause : clauses) {
        if (clause.isPattern()) {
            clauseStrategyContext.setStrategy(patternStrat);
        }
        else if (clause.isRelation()) {
            clauseStrategyContext.setStrategy(relationStrat);
        }
        else if (clause.isWith()) {
            clauseStrategyContext.setStrategy(withStrat);
        }
        else {
            assert("Invalid clause type");
        }

        if (!clauseStrategyContext.execute(clause, rdb)) {
            if (target.isBoolean()) {
                return std::unordered_set<std::string>{"FALSE"};
            }
            else {
                return {};
            }
        }
    }

    //! Insert all other variables that have not been inserted.
    for (Declaration decl : declarations) {
        insertSynonymSetIntoRDB(decl, rdb, pkb);
    }

    std::unordered_set<std::string> results = getResultsFromRDB(target, rdb);

    pkb->clearCache();

	return results;
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
        // get unique synonyms -> get unique results -> combine all unique results -> duplicate the columns based on target -> applyAttrVal to results.
        auto allSynonyms = result.target;
        std::vector<std::string> uniqueSynonyms = getSynonyms(allSynonyms);
        std::vector<std::vector<std::string>> allResults = rdb.getMultipleTarget(uniqueSynonyms);
        std::vector<std::vector<std::string>> combinedUniqueResults = combineResults(allResults);

        return addDuplicateSynonymAndApplyAttrVal(combinedUniqueResults, uniqueSynonyms, allSynonyms);
    }
    else {
        assert("False");
    }
}

std::unordered_set<std::string> QueryExecutor::addDuplicateSynonymAndApplyAttrVal(std::vector<std::vector<std::string>>& allResults,
    std::vector<std::string> uniqueSynonyms, 
    std::vector<std::variant<Declaration, AttrReference>> allSynonyms) {

    // Add the duplicate synonyms
    std::unordered_map<std::string, int> seenMap;
    std::vector<std::vector<std::string>> tempResults;

    // If they are of different size, means there are duplicate synonyms that need to be added.
    if (uniqueSynonyms.size() != allSynonyms.size()) {
        for (int i = 0; i < allResults.size(); i++) {
            tempResults.push_back(std::vector<std::string>());
        }

        for (int i = 0; i < allSynonyms.size(); i++) {
            auto syn = allSynonyms[i];
            std::string synonym;
            if (auto val = std::get_if<AttrReference>(&syn)) {
                synonym = val->declaration.name;
            }
            else if (auto val = std::get_if<Declaration>(&syn)) {
                synonym = val->name;
            }
            else {
                // No op
            }

            if (seenMap.find(synonym) == seenMap.end()) {
                // Never see before, add synonym to index mapping
                auto it = std::find(uniqueSynonyms.begin(), uniqueSynonyms.end(), synonym);
                auto idx = it - uniqueSynonyms.begin();
                seenMap.insert({ synonym, idx });
                for (int j = 0; j < allResults.size(); j++) {
                    auto val = allResults[j][idx];
                    tempResults[j].push_back(val);
                }
            }
            else {
                // Already in seen, therefore this is a duplicate. Use the current index, get the duplicate values, insert straightaway.
                // Iterate through the column
                auto index = seenMap.find(synonym)->second;
                for (int j = 0; j < allResults.size(); j++) {
                    auto currentRow = tempResults[j];
                    auto dupeVal = allResults[j][index];
                    currentRow.push_back(dupeVal);
                    tempResults[j] = currentRow;
                }
            }
        }
        allResults = tempResults;
    }

    // Apply attribute values to AttrRef
	for (int j = 0; j < allSynonyms.size(); j++) {
        auto target = allSynonyms[j];
		if (auto val = std::get_if<AttrReference>(&target)) {

            if (val->declaration.Type == Declaration::DesignEntity::Procedure ||
                val->declaration.Type == Declaration::DesignEntity::Statement ) {
                continue;
            }

            std::vector<std::string> attrResults;
            std::unordered_map<std::string, std::string> seen;
			for (int i = 0; i < allResults.size(); i++) {
                std::string oldValue = allResults[i][j];
                std::string newValue;
                if (seen.find(oldValue) == seen.end()) {
					newValue =  pkb->getValueFromKey(oldValue, val->declaration.Type, val->attr);
                    seen.insert({ oldValue, newValue });
                }
                else {
                    newValue = seen.find(oldValue)->second;
                }
                // Replace the old value with the new attribute value
                allResults[i][j] = newValue;
            }
		}
	}

    // Format and change to unordered_set
    std::unordered_set<std::string> finalFormattedResults;
    for (auto& listOfResults : allResults) {
        finalFormattedResults.insert(formatString(listOfResults));
    }

    return finalFormattedResults;
}

void QueryExecutor::insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage* pkb) {
	if (rdb.variableIsPresent(decl.name)) return;
	std::unordered_set<std::string> resultsFromPKB = pkb->getEntitySet(decl.Type);
	rdb.insertList(decl.name, resultsFromPKB);
}

std::vector<std::string> QueryExecutor::getSynonyms(std::vector<std::variant<Declaration, AttrReference>>& targets) {
    std::vector<std::string> allSynonyms;
    std::string synonym;
    std::unordered_set<std::string> set;
    for ( auto& target : targets) {
        if (auto declPtr = std::get_if<Declaration>(&target)) {
            synonym = declPtr->name;
        }
        else if (auto attrRefPtr = std::get_if<AttrReference>(&target)) {
            synonym = attrRefPtr->declaration.name;
        }

        if (set.find(synonym) == set.end()) {
            allSynonyms.push_back(synonym);
            set.insert(synonym);
        }
    }
    return allSynonyms;
}

std::vector<std::vector<std::string>> QueryExecutor::combineResults(std::vector<std::vector<std::string>> allResults) {
    // Within the string, the values are separated by comma
    std::vector<std::vector<std::string>> finalResults;
    if (allResults.size() == 0) {
        return allResults;
    }

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

    return finalResults;
}

std::string QueryExecutor::formatString(std::vector<std::string> strings) {
    std::string finalString = strings[0];
    for (int i = 1; i < strings.size(); i++) {
        finalString += " " + strings[i];
    }
    return finalString;
}
