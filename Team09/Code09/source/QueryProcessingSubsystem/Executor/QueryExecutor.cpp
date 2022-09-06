#include <string>
#include <vector>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "ModifiesEvaluator.h"

std::unordered_set<std::string>* QueryExecutor::processQuery(Query* query) {
	Relation relations = query->relations;
	std::vector<std::string> patterns = query->patterns;
	std::vector<std::string> declarations = query->declarations;
	std::string tokens = query->target;
	std::unordered_set<std::string>* results = new std::unordered_set<std::string>();
    results->insert("x");
	// Call the "Relation" execute immediately
//    std::unordered_set<std::string> temp = execute();
//	results->insert(temp.begin(), temp.end());
	return results;
}

// Relation execute
std::unordered_set<std::string> QueryExecutor::execute() {
	switch (relations.TYPE) {
	case Relation::Modifies:
		// Create the Modifies Evaluator and evaluate it
		return ModifiesEvaluator({}, relations, pkb).evaluate();
	}
}

// TODO: Pattern execute
