#include "ResultsTables.h"

void ResultsTables::create(Variable variable, std::unordered_set<Value> list) {
	columnName.push_back(variable);
	varToColIndex.insert({variable, columnIndex});

	// Insertion of elements into the matrix.
	int count = 0;
	for (auto& value : list) {
		if (count == list.size()) {
			break;
		}
		resultsTable.push_back({});
		resultsTable[count][columnIndex] = value;
		count++;
	}

	columnIndex++;
}

void ResultsTables::create(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> listPair) {
	columnName.push_back(var1);
	int firstIndex = columnIndex;
	varToColIndex.insert({var1, columnIndex});
	int secondIndex = columnIndex;
	columnName.push_back(var2);
	varToColIndex.insert({var2, columnIndex});

	// Insertion
	int count = 0;
	for (auto& value : listPair) {
		if (count == listPair.size()) {
			break;
		}
		resultsTable.push_back({});
		resultsTable[count][firstIndex] = value.first;
		resultsTable[count][secondIndex] = value.second;
		count++;
	}
}

bool ResultsTables::insertIntoTableSameSynonymList(Variable variable, std::unordered_set<Value> list) {
	int index = varToColIndex.at(variable);
	bool add = true;

	// Only values in both list and table can be kept.
	for (int i = 0; i < resultsTable.size(); i++) {
		Variable tableValue = resultsTable[i][index];

		// Value not in both table and list
		if (list.find(tableValue) == list.end()) {

			// NEEDS CHANGING: HOW TO ERASE?
			resultsTable[i][index] = "NULL";
		}
		
	}
}

bool ResultsTables::insertIntoTableNewSynonymListPair(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> listPair) {

}


bool ResultsTables::insertIntoTableSameSynonymListPair(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> listPair) {
	// Only assignments in the list AND the current table can be kept.

}

// IMPLEMENT THE INSERTER -> INSERT AT SPECIFIC ROWS OF THE TABLE.
// BUT HAVE TO TELL RY CHANGE EVERYTHING TO VECTOR. POSSIBLE?
// void ResultsTables::inserter() 

std::unordered_set<Value> getResultBySynonym(Variable variable) {
	//int index = 
}



