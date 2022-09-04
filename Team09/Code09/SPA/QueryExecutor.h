#pragma once
#include <string>
#include <vector>
#include "Query.h"
#include "Relation.h"
#include "PKB.h" // For including the PKB file

class QueryExecutor {
private:
	PKB pkb;
	Relation relations;
	std::vector<std::string> patterns;
	std::vector<std::string> declarations;
	std::vector<std::string> tokens;

	std::vector<std::string> execute(std::vector<std::string> declarations, Relation relations);

public:
	std::vector<std::string> processQuery(Query* query);
	QueryExecutor(PKB pkb) : pkb(pkb) {}; // Constructor for taking in PKB
};
