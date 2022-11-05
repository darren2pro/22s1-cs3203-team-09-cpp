#include <cassert>
#include "ResultsDatabase.h"
#include <stdexcept>
#include "../../AttrReference.h"

using namespace std;

bool ResultsDatabase::variableIsPresent(Variable var) {
    bool isPresent = find(allVariables.begin(), allVariables.end(), var) != allVariables.end();
    return isPresent;
}

bool ResultsDatabase::insertList(Variable variable, std::unordered_set<Value> list) {
	int tableIndex = getVariableIndex(variable);
	// Variable does not exist in RDB, create a new table
	if (tableIndex < 0) {
		createSingleVariableTable(variable, list);
		return true;
	}
	// Variable exists in the RDB, add to existing table
	else {
		return allResultsTables[tableIndex].insertListToTable(variable, list);
	}
}

bool ResultsDatabase::insertPairList(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PKB::pairHash> listPair) {
	int firstIndex = getVariableIndex(var1);
	int secondIndex = getVariableIndex(var2);
	
	// There are no tables with both variables already inside
	if (firstIndex == -1 && secondIndex == -1) {
		createDoubleVariableTable(var1, var2, listPair);
		return true;
	}
	// Var1 doesn't exist, but there is a table where var2 exists.
	else if(firstIndex == -1) {
		return allResultsTables[secondIndex].insertListPairToTable(var1, var2, listPair);
	}
// Var2 doesn't exist, but there is a table where var1 exists.
	else if (secondIndex == -1) {
	return allResultsTables[firstIndex].insertListPairToTable(var1, var2, listPair);
	}
	// Both variables exist in same table
	else if (firstIndex == secondIndex) {
	return allResultsTables[firstIndex].insertListPairToTable(var1, var2, listPair);
	}
	// Variables exist in 2 different tables. Need to merge the two tables together.
	else {
	combineTables(firstIndex, secondIndex);
	int newIndex = getNewTableIndexAfterCombine(var1, var2);
	return allResultsTables[newIndex].insertListPairToTable(var1, var2, listPair);
	}
}

int ResultsDatabase::getVariableIndex(Variable variable) {
	auto keyValuePair = varToIndexMap.find(variable);
	if (keyValuePair == varToIndexMap.end()) {
		return -1;
	}
	else {
		return keyValuePair->second;
	}
}

int ResultsDatabase::getNewTableIndexAfterCombine(Variable var1, Variable var2) {
	int firstIndex = getVariableIndex(var1);
	int secondIndex = getVariableIndex(var2);
	if (firstIndex == -1 || secondIndex == -1) throw runtime_error("Variables do not exist in the RDB.");
	if (firstIndex == secondIndex) {
		return firstIndex;
	}
	else {
		throw runtime_error("Variables exist in different tables after a combine has been done.");
	}
}

void ResultsDatabase::createSingleVariableTable(Variable variable, std::unordered_set<Value> list) {
	ResultsTables resultsTable;
	resultsTable.create(variable, list);
	allResultsTables.push_back(resultsTable);
	int tableIndex = allResultsTables.size() - 1;
	addNewTableToMap(variable, tableIndex);
	allVariables.push_back(variable);
}

void ResultsDatabase::createDoubleVariableTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PKB::pairHash> pairList) {
	ResultsTables resultsTable;
	resultsTable.create(var1, var2, pairList);
	allResultsTables.push_back(resultsTable);
	int tableIndex = allResultsTables.size() - 1;
	addNewTableToMap(var1, tableIndex);
	addNewTableToMap(var2, tableIndex);
	allVariables.push_back(var2);
	allVariables.push_back(var1);
}

// TableTable Join
bool ResultsDatabase::combineTables(int firstIndex, int secondIndex) {
	auto& t1 = allResultsTables[firstIndex];
	auto& t2 = allResultsTables[secondIndex];

	// Assuming joining table into t1. Remove table 2 index.
	bool result = t1.combineTableWith(t2);
	removeTable(secondIndex);
	return result;
}

std::vector<int> ResultsDatabase::getAllLinkedIndices(int index, std::vector<std::string> uniqueSynonyms) {
	std::string synonym = uniqueSynonyms[index];
	int tableIndex = varToIndexMap.find(synonym)->second;

	// create a variable to int map
	std::unordered_map<std::string, int> map;
	for (int i = 0; i < uniqueSynonyms.size(); i++) {
		map.insert({ uniqueSynonyms[i], i });
	}

	std:vector<int> indices;
	for (const auto pair : varToIndexMap) {
		if (pair.second == tableIndex && map.find(pair.first) != map.end()) {
			int foundInt = map.find(pair.first)->second;
			indices.push_back(foundInt);
		}
	}
	return indices;
}

void ResultsDatabase::removeTable(int index) {
	// Remove the table from allResultsTable
	auto iterator = allResultsTables.begin();
	allResultsTables.erase(iterator + index);

	varToIndexMap.clear();
	for (int currentTableIdx = 0; currentTableIdx < allResultsTables.size(); currentTableIdx++) {
		for (auto [var_name, col_idx] : allResultsTables[currentTableIdx].varToColIndex) {
			varToIndexMap.insert({ var_name, currentTableIdx });
		}
	}
}

std::unordered_set<std::string> ResultsDatabase::getResults(Declaration& target) {
	int tableIndex = getVariableIndex(target.name);
	ResultsTables rt = allResultsTables[tableIndex];
	std::vector<Value> vecResults = rt.getResultBySynonym(target.name);

	// This is only called by single-var ops. Just return unique values.
	std::unordered_set<Value> setResults;
	for (auto val : vecResults) {
		setResults.insert(val);
	}
	return setResults;
}

std::vector<std::vector<std::string>> ResultsDatabase::getMultipleTarget(std::vector<std::string> allSynonyms) {
	// accept as parameter, an empty vector (1 for each synonym). Place each result vector into the respective slots.
	std::vector<std::vector<std::string>> finalResults(allSynonyms.size());
	fill(finalResults.begin(), finalResults.end(), std::vector<std::string>());


	for (auto& table : allResultsTables) {
		// finalResults is edited inside.
		std::vector<std::vector<std::string>> tempResults = table.getResultByMultipleSynonym(allSynonyms, finalResults);
	}

	return finalResults;
}

void ResultsDatabase::addNewTableToMap(Variable variable, int tableIndex) {
	varToIndexMap[variable] = tableIndex;
}

bool ResultsDatabase::getBoolean() {
	for (ResultsTables& table : allResultsTables) {
		if (table.resultsTable.size() == 0) {
			return false;
		}
	}
	return true;
}
