#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "ResultsTables.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../Declaration.h"
#include "../../AttrReference.h"

typedef std::string Variable;
typedef std::string Value;
typedef std::string TableIndex;

using namespace PKB;

class ResultsDatabase {
public:
    /**
     * All variables that are present in the rdb.
     */
	std::vector<Variable> allVariables;
	std::vector<ResultsTables> allResultsTables;
	std::unordered_map<Variable, int> varToIndexMap;
	bool validQuery = true; // Default to true. If any clause returns false, final result will be invalid query.

	ResultsDatabase() :
		allVariables(std::vector<Variable>()),
		allResultsTables(std::vector<ResultsTables>()) {};

	// Methods for fetching results
	bool getBoolean();
	std::vector<std::vector<std::string>> getMultipleTarget(std::vector<std::string> allSynonyms);

    /**
     * Checks whether the given variable already exists within this results database. Being present here means
     * that they have their constrained values in one of the results tables, and have a mapping from variable to index in the varToIndexMap.
     */
    bool variableIsPresent(Variable var);
    /**
     * Get index of variable if it exists in this results database. This index is referencing a single table in allResultsTables.
     * If the variable does not exist in this results database, this method returns -1.
     */
    int getVariableIndex(Variable variable);
    /**
     * Insert the given variable with the given list of values that it can take into the results database.<br><br>
     * This method calls createSingleVariableTable or ResultTable::insertListToTable depending on whether the variable
     * already exists in the results database.
     * @param variable variable string to insert into the results database
     * @param list unordered set of possible values that this variable can take
     * @return false if the resulting modified table is empty, returns true otherwise
     */
	bool insertList(Variable variable, std::unordered_set<Value> list);
	bool insertPairList(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, pairHash> listPair);

	//! Return the correct index after table combine. The way we do it is to retrieve the mappings for the
	//! two variables again. If the index is different, a runtime_error is thrown.
	int getNewTableIndexAfterCombine(Variable var1, Variable var2);

	//! Adds a new mapping from variable to tableIdx. This tableIdx is referencing allResultsTables.
	void addNewTableToMap(Variable variable, int tableIndex);

	/**
	 * Create a new table with the given variable and list of possible values that it can take. This method also
	 * updates the varToIndexMap to remember the mapping from variable to table index, then stores the variable name
	 * in the allVariables vector.
     */
	void createSingleVariableTable(Variable variable, std::unordered_set<Value> list);
	void createDoubleVariableTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, pairHash> listPair);

    //! Conbines the two tables into a single table. At the end of this method, only the table for firstIndex will remain.
	bool combineTables(int firstIndex, int secondIndex);

	//! Removes the resultTable at this index, and re-computes the mapping for all the variables, so that
	//! they continue to point to the correct table.
    void removeTable(int index);

	//! Fetching results
	std::unordered_set<std::string> getResults(Declaration& target);
};
