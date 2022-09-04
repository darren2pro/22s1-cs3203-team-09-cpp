#include <string>
#include <vector>
#include "QueryExecutor.h"
#include "Relation.h"
#include "ModifiesEvaluator.h"

std::vector<std::string> QueryExecutor::processQuery(Query* query) {
	Relation relations = query->relations;
	std::vector<std::string> patterns = query->patterns;
	std::vector<std::string> declarations = query->declarations;
	std::string tokens = query->target;
	std::vector<std::string> results;

	// Call the "Relation" execute immediately
	results = execute(query->declarations, query->relations);

	return results;
}

// Relation execute
std::vector<std::string> QueryExecutor::execute(std::vector<std::string> declarations, Relation relations) {
	std::vector<std::string> results;
	
	switch (relations.TYPE) {
	case Relation::Modifies:
		// Create the Modifies Evaluator and evaluate it
		return ModifiesEvaluator(declarations, relations, pkb).evaluate();
	}
}

// TODO: Pattern execute
