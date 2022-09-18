#include "ResultsTables.h"

void ResultsTables::create(Variable variable, std::unordered_set<Value>& list) {
	columnName.push_back(variable);
	varToColIndex.insert({variable, columnIndex});
	columnIndex++;

	// Insertion of elements into the matrix.
	for (const auto& value : list) {
		resultsTable.push_back({ value });
	}
}

void ResultsTables::create(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash>& listPair) {
	columnName.push_back(var1);
	int firstIndex = columnIndex;
	varToColIndex.insert({var1, columnIndex});
	columnIndex++;
	columnName.push_back(var2);
	int secondIndex = columnIndex;
	varToColIndex.insert({var2, columnIndex});

	for (auto& value : listPair) {
		resultsTable.push_back({ value.first, value.second });
	}
}

bool ResultsTables::insertListToTable(Variable variable, std::unordered_set<Value>& list) {
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
	if (resultsTable.size() <= 0) {
		return false;
	}
	else {
		return true;
	}
}

bool ResultsTables::insertListPairToTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash>& listPair) {
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
	if (resultsTable.size() <= 0) {
		return false;
	}
	else {
		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

bool ResultsTables::combineTableWith(ResultsTables& otherTable, Variable var) {
	std::unordered_map<std::string, std::vector<int>> col_hash = hashColumn(var);
	std::vector<std::vector<std::string>> out_table;
	size_t other_table_join_var_index = otherTable.varToColIndex.at(var);

	for (auto row : otherTable.resultsTable) {
		// Search for the incoming join val in the existing table
		auto join_val_other_table = row[other_table_join_var_index];
		if (col_hash.find(join_val_other_table) == col_hash.end()) continue;

		// Found it - "cross product" incoming row with the rows in original table
		row.erase(row.begin() + other_table_join_var_index);  // remove join col
		auto matching_row_indexs = col_hash[join_val_other_table];
		for (auto row_index : matching_row_indexs) {
			std::vector<std::string> original_row = resultsTable[row_index];
			original_row.insert(original_row.end(), row.begin(), row.end());
			out_table.push_back(original_row);  // inserts joined row
		}
	}

	// ** Important -> table is REPLACED with the one with just constructed
	resultsTable = out_table;
	int base_t2_column_index = columnIndex;
	// Update column names from joined table
	for (auto [name, index] : otherTable.varToColIndex) {
		if (name != var) {
			size_t new_col_index = index + base_t2_column_index;
			if (otherTable.varToColIndex.at(name) > other_table_join_var_index) {
				// If the column being merged is AFTER the join column, subtract one
				// from col index when merging.
				new_col_index--;
			}
			varToColIndex[name] = new_col_index;
			columnIndex++;  // to be consistent with its purpose
		}
	}

	// If size is 0 - no results
	return resultsTable.size() > 0;
}

std::unordered_map<std::string, std::vector<int>> ResultsTables::hashColumn(
	Variable var) {
	int variableIndex = varToColIndex[var];
	std::unordered_map<std::string, std::vector<int>> out_map;
	int index = 0;
	for (auto& row : resultsTable) {
		auto join_val = row[variableIndex];
		if (out_map.find(join_val) == out_map.end()) {
			out_map.insert({ join_val, {index} });
		}
		else {
			out_map[join_val].push_back(index);
		}
		index++;
	}

	return out_map;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

std::unordered_set<Value> ResultsTables::getResultBySynonym(Variable variable) {
	std::unordered_set<Value> finalResults;
	int index = varToColIndex[variable];
	for (auto& row : resultsTable) {
		finalResults.insert(row[index]);
	}

	return finalResults;
	//size_t col_index = name_column_map[name];
	//vector<string> out_column;
	//out_column.reserve(table.size());
	//for (auto& row : table) {
	//	out_column.push_back(row[col_index]);
	//}
	//return out_column;
}



