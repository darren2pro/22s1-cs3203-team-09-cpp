#pragma once
#include <unordered_set>
#include <string>
#include <unordered_map>
#include "../../../ProgramKnowledgeBase/PKBStorage.h"

typedef std::string Variable;
typedef std::string Value;
typedef std::string TableIndex;

using namespace PKB;

class ResultsTables {
public:
	int columnIndex = 0;
	std::vector<std::vector<Value>> resultsTable;
	std::unordered_map<Variable, int> varToColIndex;
	std::vector<Variable> columnName;

	ResultsTables() {};

	//! Create new tables for list and pairLists. This method modifies columnName vector to now remember this variable name.
	//! It also modifies the varToColIndex map to remember the mapping from variable to column index. Finally it pushes all the
	//! values from the given list into this resultsTable.
	void create(Variable variable, std::unordered_set<Value>& list);
	void create(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, pairHash>& list);

	/**
	 * Adds the given list of values to this results table. This is for a single variable list.<br><br>
	 * Returns false if the resulting modified results table is empty, returns true otherwise.
	 */
    bool insertListToTable(Variable variable, std::unordered_set<Value>& list);

	// Adding double var listpairs
	bool insertListPairToTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, pairHash>& listPair);

	//! Combine with another table
	bool combineTableWith(ResultsTables& otherTable);


	//std::unordered_map<std::string, std::vector<int>> hashColumn(Variable var);

	//! Fetching results
	std::unordered_set<Value> getResultBySynonym(Variable variable);
};
