#pragma once
#include <string>
#include <vector>
#include "Query.h"
#include "Relation.h"

class QueryExecutor {
private:
	Relation relations;
	std::vector<std::string> patterns;
	std::vector<std::string> declarations;
	std::vector<std::string> tokens;

	std::vector<std::string> execute(std::vector<std::string> declarations, Relation relations);

public:
	std::vector<std::string> processQuery(Query* query);
};
