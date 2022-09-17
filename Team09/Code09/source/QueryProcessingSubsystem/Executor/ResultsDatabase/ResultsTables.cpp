#include "ResultsTables.h"

void ResultsTables::create(Variable variable, std::unordered_set<Value> list) {
	columnName.push_back(variable);
	varToColIndex.insert({variable, columnIndex});
	columnIndex++;

	// Insertion of elements into the matrix.
	for (const auto& value : list) {
		resultsTable.push_back({ value });
	}
}

void ResultsTables::create(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> listPair) {
	columnName.push_back(var1);
	int firstIndex = columnIndex;
	varToColIndex.insert({var1, columnIndex});

	columnName.push_back(var2);
	int secondIndex = columnIndex;
	varToColIndex.insert({var2, columnIndex});

	for (auto& value : listPair) {
		resultsTable.push_back({ value.first, value.second });
	}
}

bool ResultsTables::insertListToTable(Variable variable, std::unordered_set<Value> list) {
	int index = varToColIndex[variable];
	auto remover = std::remove_if(resultsTable.begin(), resultsTable.end(),

		// Capture by reference so that the row can be changed
		[&](std::vector<std::string>& row) {
			Value value = row[index];
			bool found = list.find(value) == list.end();
			return found;
		});

	resultsTable.erase(remover, resultsTable.end());
	
	// To check if Table is empty.
	if (resultsTable.size() < 0) {
		return false;
	}
	else {
		return true;
	}
}

bool ResultsTables::insertListPairToTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>> listPair) {
	int index1 = varToColIndex[var1];
	int index2 = varToColIndex[var2];

	auto remover = std::remove_if(resultsTable.begin(), resultsTable.end(),
		[&](std::vector<std::string>& row) {
			std::string val1 = row[index1];
			std::string val2 = row[index2];
			bool found = listPair.find({ val1, val2 }) == listPair.end();
			return found;
		});

	resultsTable.erase(remover, resultsTable.end());
	
	// To check if Table is empty.
	if (resultsTable.size() < 0) {
		return false;
	}
	else {
		return true;
	}
}

std::unordered_set<Value> ResultsTables::getResultBySynonym(Variable variable) {
	std::unordered_set<Value> finalResults;
	int index = varToColIndex[variable];
	for (auto& row : resultsTable) {
		finalResults.insert(row[index]);
	}

	return finalResults;
	//size_t col_idx = name_column_map[name];
	//vector<string> out_column;
	//out_column.reserve(table.size());
	//for (auto& row : table) {
	//	out_column.push_back(row[col_idx]);
	//}
	//return out_column;
}



