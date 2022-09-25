#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../Query.h"
#include "../Relation.h"
#include "../../ProgramKnowledgeBase/PKBManager.h"
#include "ResultsDatabase/ResultsDatabase.h"

class QueryExecutor {
private:
	PKBManager pkb;
	ResultsDatabase rdb;
	Relation relations;
	Pattern pattern;
	Declaration target;
	std::vector<Declaration> declarations;
	std::vector<std::string> tokens;

	bool execute(Pattern pattern, ResultsDatabase& rdb);
	bool execute(Relation relation, ResultsDatabase& rdb);

public:
	QueryExecutor(PKBManager pkb) : pkb(pkb) {}; // Constructor for taking in PKB

	std::unordered_set<std::string> processQuery(Query* query);
	std::unordered_set<std::string> getResultsFromRDB(Declaration target, ResultsDatabase& rdb);
	/**
	 * If the rdb already contains constraints on this given declaration, then simply return without doing anything.
	 * If the rdb does not contain any constaints on this given declaration, then retrieve the full set of possible values
	 * for this declaration and insert them into the rdb.
	 */
    static void insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBManager& pkb);

};
