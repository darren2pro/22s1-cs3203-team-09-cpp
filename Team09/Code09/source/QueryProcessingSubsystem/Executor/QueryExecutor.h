#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../Query.h"
#include "../Relation.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class QueryExecutor {
private:
	PKBStorage pkb;
	Relation relations;
	std::vector<std::string> patterns;
	std::vector<std::string> declarations;
	std::vector<std::string> tokens;

	std::unordered_set<std::string> execute();

public:
	std::unordered_set<std::string> processQuery(Query* query);
	QueryExecutor(PKBStorage pkb) : pkb(pkb) {}; // Constructor for taking in PKB
};
