#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "ResultsTables.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../Declaration.h"

typedef std::string Variable;
typedef std::string Value;
typedef std::string TableIndex;

using namespace PairHasher;

class ResultsDatabase {
public:
	std::vector<Variable> allVariables;
	std::vector<ResultsTables> allResultsTables;
	std::unordered_map<Variable, int> varToIndexMap;
	bool validQuery = true; // Default to true. If any clause returns false, final result will be invalid query.

	ResultsDatabase() :
		allVariables(std::vector<Variable>()),
		allResultsTables(std::vector<ResultsTables>()) {};

	bool insertList(Variable variable, std::unordered_set<Value> list);
	bool insertPairList(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash> listPair);

	// Get index of variable if it exists in tables
	int getVariableIndex(Variable variable);

	// Adding new table index into the map
	void addNewTableToMap(Variable variable, int tableIndex);

	// Creating of tables
	void createSingleVariableTable(Variable variable, std::unordered_set<Value> list);
	void createDoubleVariableTable(Variable var1, Variable var2, std::unordered_set<std::pair<Value, Value>, PairHasher::pairHash> listPair);

	bool combineTables(int firstIndex, int secondIndex);
	void removeTable(int index);

	// Fetching results
	std::unordered_set<std::string> getResults(Declaration& target);

	//// Dealing with incoming results
	//bool mergeListList(std::vector<std::string> list1, std::vector<std::string> list2);
	//bool mergePairListPairList(std::vector<std::pair<std::string, std::string>>)
};
