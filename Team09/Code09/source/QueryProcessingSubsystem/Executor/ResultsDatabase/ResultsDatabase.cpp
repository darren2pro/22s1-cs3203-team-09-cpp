#include <cassert>
#include "ResultsDatabase.h"

using namespace std;

bool ResultsDatabase::variableIsPresent(Variable var) {
    const bool isPresent = find(allVariables.begin(), allVariables.end(), var) != allVariables.end();
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

bool ResultsDatabase::insertPairList(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash> listPair) {
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
		//! For query 12: Here first index is 1, but it needs to be zero. Need to find a way to get the updated index? E.g. combine tables return an index or something.
		// Indices are correct. Same index because they point to the same table.


		// After combining tables, firstIndex and secondIndex should point to the same table.

		// Need to check which index is the new index.

		int newIndex = getNewTableIndexAfterCombine(firstIndex, secondIndex);

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

int ResultsDatabase::getNewTableIndexAfterCombine(int firstIndex, int secondIndex) {
	// Go thru variableToTableMap. The invalid pointer should not be inside the map.

	for (auto& pair : varToIndexMap) {
		if (pair.second == firstIndex) {
			return firstIndex;
		}
		else if (pair.second == secondIndex) {
			return secondIndex;
		}
		else {
			assert("Error: Neither indices are found in varToIndexMap");
		}
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

void ResultsDatabase::createDoubleVariableTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash> pairList) {
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
	
	// RESULTS NOT SAVED.

	//// Point all the variables in t2 to the combine table index.
	//for (auto& var : t2.columnName) {
	//	varToIndexMap[var] = firstIndex;
	//}
	removeTable(secondIndex);
	
	// TODO: 
	// Chuan you can try to return the index of the new table here because one of the table was removed already so you need to re-map all the other tables in the
	// varToIndexMap to the new index. Like if you remove table 2, then table 3 will be at index 2, table 4 will be at index 3, etc. Need to map for all the respective
	// variables I think

	return result;
}

void ResultsDatabase::removeTable(int index) {
	// Remove the table from allResultsTable
	auto iterator = allResultsTables.begin();
	allResultsTables.erase(iterator + index);

	varToIndexMap.clear();
	for (int i = 0; i < allResultsTables.size(); i++) {
		for (auto [var_name, col_idx] : allResultsTables[i].varToColIndex) {
			varToIndexMap.insert({ var_name, i });
		}
	}
}

std::unordered_set<std::string> ResultsDatabase::getResults(Declaration& target) {
	if (validQuery) {
		int tableIndex = getVariableIndex(target.name);
		ResultsTables rt = allResultsTables[tableIndex];
		auto results = rt.getResultBySynonym(target.name);
		return results;
	}
	else {
		return { "Invalid Query" };
	}

}

void ResultsDatabase::addNewTableToMap(Variable variable, int tableIndex) {
	varToIndexMap[variable] = tableIndex;
}
