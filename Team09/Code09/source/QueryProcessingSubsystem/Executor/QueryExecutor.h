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

	std::unordered_set<std::string> processQuery(Query* query);

	/**
	 * Used to insert all the possible values for this declaration into the ResultsDatabase.
	 */
	static void insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage* pkb);

	std::unordered_set<std::string> getResultsFromRDB(Result target, ResultsDatabase& rdb);

	std::unordered_set<std::string> addDuplicateSynonymAndApplyAttrVal(std::vector<std::vector<std::string>>& allResults, std::vector<std::string> uniqueSynonyms, std::vector<std::variant<Declaration, AttrReference>> targets);

	std::vector<std::string> getSynonyms(std::vector<std::variant<Declaration, AttrReference>>& targets);

	std::vector<std::vector<std::string>> combineResults(std::vector<std::vector<std::string>> allResults, std::vector<std::string> uniqueSynonyms);

	std::string formatString(std::vector<std::string> strings);

	/**
	 * These methods are to help with the integration testing.
	*/
	std::vector<Relation> getRelations() { return relations; }
	std::vector<Pattern> getPatterns() { return pattern; }
	std::vector<With> getWiths() { return with; }
	Result getTarget() { return target; }
	std::vector<Declaration> getDeclarations() { return declarations; }
};
