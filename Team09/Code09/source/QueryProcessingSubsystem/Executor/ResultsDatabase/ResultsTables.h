#pragma once
#include <unordered_set>
#include <string>
#include <unordered_map>
#include "../../../ProgramKnowledgeBase/PKBStorage.h"

typedef std::string Variable;
typedef std::string Value;
typedef std::string TableIndex;

using namespace PairHasher;

class ResultsTables {
public:
	int columnIndex = 0;
	std::vector<std::vector<Value>> resultsTable;
	std::unordered_map<Variable, int> varToColIndex;
	std::vector<Variable> columnName;

	ResultsTables() {};

	// Create new tables for list and pairLists
	void create(Variable variable, std::unordered_set<Value>& list);
	void create(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash>& list);

	// Modification to current tables
	// Adding single var list
	bool insertListToTable(Variable variable, std::unordered_set<Value>& list);

	// Adding double var listpairs
	bool insertListPairToTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash>& listPair);

	// Combine with another table
	bool combineTableWith(ResultsTables& otherTable, Variable var);


	std::unordered_map<std::string, std::vector<int>> hashColumn(Variable var);

	// Fetching results
	std::unordered_set<Value> getResultBySynonym(Variable variable);
};
