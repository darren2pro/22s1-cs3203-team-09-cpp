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

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
    relations = query->relations;
	pattern = query->patterns;
    with = query->withs;
	declarations = query->declarations;
	target = query->target;
	rdb = ResultsDatabase();

    std::shared_ptr<WithStrategy> withStrat = std::make_shared<WithStrategy>(declarations, pkb);
    std::shared_ptr<PatternStrategy> patternStrat = std::make_shared<PatternStrategy>(declarations, pkb);
    std::shared_ptr<RelationStrategy> relationStrat = std::make_shared<RelationStrategy>(declarations, pkb);
    ClauseStrategyContext clauseStrategyContext(withStrat);

    std::vector<Clause> clauses = ClausePrioritizer(query).getClauses();

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
        std::vector<std::string> uniqueSynonyms = getUniqueSynonyms(allSynonyms);
        std::vector<std::vector<std::string>> allResults = rdb.getMultipleTarget(uniqueSynonyms);
        std::vector<std::vector<std::string>> combinedUniqueResults = combineResults(allResults, uniqueSynonyms);

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
                // Add synonym to index mapping
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
            auto deType = val->declaration.Type;
            auto attrType = val->attr;

            if (!(deType == Declaration::DesignEntity::Call && attrType == AttrReference::Attribute::ProcName ||
                deType == Declaration::DesignEntity::Read && attrType == AttrReference::Attribute::VarName ||
                deType == Declaration::DesignEntity::Print && attrType == AttrReference::Attribute::VarName)) {
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

std::vector<std::string> QueryExecutor::getUniqueSynonyms(std::vector<std::variant<Declaration, AttrReference>>& targets) {
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

std::vector<std::vector<std::string>> QueryExecutor::combineResults(std::vector<std::vector<std::string>> allResults, std::vector<std::string> uniqueSynonyms) {
    // Within the string, the values are separated by comma
    std::vector<std::vector<std::string>> finalResults;

    // Optimized slightly for 0 and 1 synonyms
    if (allResults.size() == 0) {
        return allResults;
    }

    if (allResults.size() == 1) {
        std::vector<std::vector<std::string>> finalResults;
        std::unordered_set<std::string> seen;
		for (auto val : allResults[0]) {
            if (seen.find(val) == seen.end()) {
				finalResults.push_back({ val });
                seen.insert(val);
            }
		}
        return finalResults;
    }

	// Initialize empty row of max possible size.
	std::vector<std::string> v(allResults.size());
	fill(v.begin(), v.end(), "");
	finalResults.push_back(v);

    // seen Set to keep track of synonyms to prevent repeats
    std::unordered_set<int> seenSynonym;

    for (int i = 0; i < allResults.size(); i++) { 
        // If synonym has been seen before, means it has been already permutated.
        if (seenSynonym.find(i) != seenSynonym.end()) continue;

		std::vector<std::vector<std::string>> newFinalResults;
		for (int k = 0; k < finalResults.size(); k++) { // for each final "string"
            // This is the base string that is being permutated on.
            auto temp = finalResults[k]; 

			// For every one of its paired values, 
			int baseIndex = i;
			std::vector<int> allIndices = rdb.getAllLinkedIndices(baseIndex, uniqueSynonyms);

			for (int j = 0; j < allResults[i].size(); j++) { // each synonym's values
				auto constant = temp; // Just a copy

                for (auto index : allIndices) {
                    // Set the correct value at the correct index for variables linked to current Synonym
                    constant[index] = allResults[index][j];
                }
				newFinalResults.push_back(constant);
			}

            // add every synonym added this iteration into seenSynonym
            for (auto index : allIndices) {
                seenSynonym.insert(index);
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
