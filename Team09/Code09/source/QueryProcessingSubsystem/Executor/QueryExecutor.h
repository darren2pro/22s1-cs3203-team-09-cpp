#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../Query.h"
#include "../Relation.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class QueryExecutor {
private:
	PKB::PKBStorage pkb;
	Relation relations;
	Pattern pattern;
	Declaration target;
	std::vector<Declaration> declarations;
	std::vector<std::string> tokens;

	auto execute(Pattern pattern);
	auto execute(Relation relation);

public:
	std::unordered_set<std::string> processQuery(Query* query);
	QueryExecutor(PKB::PKBStorage pkb) : pkb(pkb) {}; // Constructor for taking in PKB
};
