#include <cassert>
#include "ResultsDatabase.h"

bool ResultsDatabase::insertList(Variable variable, std::unordered_set<Value> list) {
	int index = getVariableIndex(variable);
	// Variable does not exist in RDB, return a new table index.
	if (index < 0) {
		// Create a new table
		createSingleVariableTable(variable, list);
		return true;
	}
	// Variable exists in the RDB, return the table index now.
	else {
		return allResultsTables[index].insertIntoTableSameSynonymList(variable, list);
	}
}

bool ResultsDatabase::insertPairList(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> listPair) {
	int firstIndex = getVariableIndex(var1);
	int secondIndex = getVariableIndex(var2);
	
	// There are no tables with both variables already inside
	if (firstIndex == -1 && secondIndex == -1) {
		createDoubleVariableTable(var1, var2, listPair);
	}
	// Var1 doesn't exist, but there is a table where var2 exists.
	else if(firstIndex == -1) {
		return allResultsTables[secondIndex].insertIntoTableNewSynonymListPair(var1, var2, listPair);
	}
	// Var2 doesn't exist, but there is a table where var1 exists.
	else if (secondIndex == -1) {
		return allResultsTables[firstIndex].insertIntoTableNewSynonymListPair(var1, var2, listPair);
	}
	// Both variables exist in same table
	else if (firstIndex == secondIndex) {
		return allResultsTables[firstIndex].insertIntoTableSameSynonymListPair(var1, var2, listPair);
	}
	// Variables exist in 2 different tables. Need to merge the two tables together.
	else {
		assert("Not needed for milestone 1");
	}
	

}

bool ResultsDatabase::insertBoolean(bool boolean) {
	validQuery = boolean;
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

bool ResultsDatabase::createSingleVariableTable(Variable variable, std::unordered_set<Value> list) {
	ResultsTables resultsTable;
	resultsTable.create(variable, list);
	allResultsTables.push_back(resultsTable);
	int tableIndex = allResultsTables.size();
	addNewTableToMap(variable, tableIndex);
}

bool ResultsDatabase::createDoubleVariableTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> pairList) {
	ResultsTables resultsTable;
	resultsTable.create(var1, var2, pairList);
	allResultsTables.push_back(resultsTable);
	int tableIndex = allResultsTables.size();
	addNewTableToMap(var1, tableIndex);
	addNewTableToMap(var2, tableIndex);
}

std::unordered_set<std::string> ResultsDatabase::getResults(Declaration target) {
	if (validQuery) {
		int tableIndex = getVariableIndex(target.name);
		return allResultsTables[tableIndex].getResultBySynonym(target);
	}
	else {
		return { "Invalid Query" };
	}

}

void ResultsDatabase::addNewTableToMap(Variable variable, int tableIndex) {
	varToIndexMap[variable] = tableIndex;
}
