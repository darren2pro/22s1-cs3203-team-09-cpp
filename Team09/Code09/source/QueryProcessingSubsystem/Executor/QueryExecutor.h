#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <variant>
#include "../Query.h"
#include "../Relation.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include "ResultsDatabase/ResultsDatabase.h"

class QueryExecutor {
private:
	PKBStorage* pkb;
	ResultsDatabase rdb;
	std::vector<Relation> relations;
	std::vector<Pattern> pattern;
	std::variant<Declaration, AttrReference> target;
	std::vector<Declaration> declarations;
	std::vector<std::string> tokens;

	bool execute(Pattern pattern, ResultsDatabase& rdb);
	bool execute(Relation relation, ResultsDatabase& rdb);

public:
	QueryExecutor(PKBStorage* pkb) : pkb(pkb) {}; // Constructor for taking in PKB

	std::unordered_set<std::string> processQuery(Query* query);

    /**
     * Used to insert all the possible values for this declaration into the ResultsDatabase.
     */
    static void insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage* pkb);
	std::unordered_set<std::string> getResultsFromRDB(std::variant<Declaration, AttrReference> target, ResultsDatabase& rdb);
	/**
	 * If the rdb already contains constraints on this given declaration, then simply return without doing anything.
	 * If the rdb does not contain any constaints on this given declaration, then retrieve the full set of possible values
	 * for this declaration and insert them into the rdb.
	 */
    //static void insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage& pkb); comment out just for reference. Need to be removed.

};
