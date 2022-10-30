#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <variant>
#include "../Query.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include "ResultsDatabase/ResultsDatabase.h"

class QueryExecutor {
private:
	PKBStorage* pkb;
	ResultsDatabase rdb;
	std::vector<Relation> relations;
	std::vector<Pattern> pattern;
	std::vector<With> with;
	Result target;
	std::vector<Declaration> declarations;
	std::vector<std::string> tokens;

	bool patternExecute(Pattern pattern, ResultsDatabase& rdb);
	bool relationExecute(Relation relation, ResultsDatabase& rdb);
	bool withExecute(With with, ResultsDatabase& rdb);

public:
	QueryExecutor(PKBStorage* pkb) : pkb(pkb) {}; // Constructor for taking in PKB

	std::unordered_set<std::string> processQuery(Query* query, bool performOptimized);

	/**
	 * Used to insert all the possible values for this declaration into the ResultsDatabase.
	 */
	static void insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage* pkb);

	std::unordered_set<std::string> getResultsFromRDB(Result target, ResultsDatabase& rdb);

	std::vector<std::string> getSynonyms(std::vector<std::variant<Declaration, AttrReference>>& targets);

	std::unordered_set<std::string> combineResults(std::vector<std::vector<std::string>> allResults);

	std::string formatString(std::vector<std::string> strings);
};
