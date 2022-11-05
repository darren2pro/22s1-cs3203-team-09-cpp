#include "ResultsTables.h"

void ResultsTables::create(Variable variable, std::unordered_set<Value>& list) {
	columnName.push_back(variable);
	varToColIndex.insert({variable, columnIndex});
	columnIndex++;

	for (auto& value : list) {
		resultsTable.push_back({ value });
	}
}

void ResultsTables::create(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PKB::pairHash>& listPair) {
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
	int colIndex = varToColIndex[variable];
    //! This remover lambda expression together with the erase function call is used to remove any element currently
    //! in this resultsTable that is not in the list which is being passed as argument.
	auto remover = std::remove_if(resultsTable.begin(), resultsTable.end(),
		// Capture by reference so that the row can be changed
		[&](std::vector<std::string>& row) {
			Value valueCurrentlyPresent = row[colIndex];
			bool notFound = list.find(valueCurrentlyPresent) == list.end();
			return notFound;
		});
	resultsTable.erase(remover, resultsTable.end());
	
	if (resultsTable.size() <= 0) {
		return false;
	}
	else {
		return true;
	}
}

bool ResultsTables::insertListPairToTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PKB::pairHash>& listPair) {
	int colIdxVar1 = varToColIndex[var1];
	int colIdxVar2 = varToColIndex[var2];

    //! Removes any element currently in this resultsTable that is not in the listPair which is being passed as argument.
	auto remover = std::remove_if(resultsTable.begin(), resultsTable.end(),
		[&](std::vector<std::string>& row) {
			std::string val1 = row[colIdxVar1];
			std::string val2 = row[colIdxVar2];
			bool notFound = listPair.find({val1, val2 }) == listPair.end();
			return notFound;
		});
	resultsTable.erase(remover, resultsTable.end());
	
	if (resultsTable.size() <= 0) {
		return false;
	}
	else {
		return true;
	}
}

// My implementation. Consider changing after MS1
bool ResultsTables::combineTableWith(ResultsTables& otherTable) {

	std::vector<std::vector<std::string>> new_table;

	for (std::vector<Variable> row1 : resultsTable) {
		for (std::vector<Variable> row2 : otherTable.resultsTable) {
			std::vector<Variable> temp;
			for (int i = 0; i < row1.size(); i++)
				temp.push_back(row1[i]);

			for (int i = 0; i < row2.size(); i++)
				temp.push_back(row2[i]);

			new_table.push_back(temp);
		}
	}

	// Replace the current table with new table.
	resultsTable = new_table; 

	// Change variable to col mapping. Table 1 variables have no change. 
	std::vector<Variable> varInTable2 = otherTable.columnName;
	for (Variable var : varInTable2) {
		varToColIndex[var] = columnIndex;
		columnName.push_back(var);
		columnIndex++;
	}


	return resultsTable.size() > 0;
}

std::vector<Value> ResultsTables::getResultBySynonym(Variable variable) {
	std::vector<Value> finalResults;
	int index = varToColIndex[variable];

	for (auto& row : resultsTable) {
		finalResults.push_back(row[index]);
	}

	return finalResults;
}

std::vector<std::vector<std::string>> ResultsTables::getResultByMultipleSynonym(
	std::vector<std::string> allSynonyms,
	std::vector<std::vector<std::string>>& finalResults) {
	bool isSingleVar = allSynonyms.size() <= 1;
	for (int i = 0; i < allSynonyms.size(); i++) {
		auto synonym = allSynonyms[i];

		// check if synonym in columnName vector
		if (std::find(columnName.begin(), columnName.end(), synonym) == columnName.end()) continue;

		auto tempResults = getResultBySynonym(synonym);
		if (tempResults.size() == 0) continue;

		// change unordered set into vectors
		// special operation just for tuples, where order matters.
		std::vector<std::string> vec;
		for (auto& s : tempResults) {
			vec.push_back(s);
		}
		finalResults[i] = vec;
	}

	return finalResults;
}



