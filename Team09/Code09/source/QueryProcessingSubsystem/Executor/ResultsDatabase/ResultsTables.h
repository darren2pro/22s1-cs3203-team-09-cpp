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
	 *
	 * <br><br>Essentially constraints the values of the variable in this results table some more. I.e. narrows the possible
	 * values some more.
	 */
    bool insertListToTable(Variable variable, std::unordered_set<Value>& list);

	/**
	 * Adds the listPair to this result table. It is assumed that the two variables passed as arguments are indeed present
	 * in this results table. If they are not present, there will be an error.
	 *
	 * <br><br>Returns false if the resulting modified results table is empty, returns true otherwise.
	 *
	 *  <br><br>Essentially constraints the values in this table some more. I.e. narrows the possible values that the
	 *  synonyms can take some more
	 */
	bool insertListPairToTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, pairHash>& listPair);

	//! Does a cross product across the two tables so that the two tables merge into this table.
	//! At the end of this operation, only this table will survive.
	bool combineTableWith(ResultsTables& otherTable);

	//! Fetching results
	std::unordered_set<Value> getResultBySynonym(Variable variable);

	std::vector<std::vector<std::string>> getResultByMultipleSynonym(std::vector<std::string> allSynonyms);
};
