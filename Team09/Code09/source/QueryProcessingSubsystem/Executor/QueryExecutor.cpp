#include <string>
#include <vector>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "ModifiesEvaluator.h"

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	std::vector<std::string> patterns = query->patterns;
	std::vector<std::string> declarations = query->declarations;
	std::string tokens = query->target;
	std::unordered_set<std::string> results;
	// Call the "Relation" execute immediately
	results = execute();
	return results;
}

// Relation execute
std::unordered_set<std::string> QueryExecutor::execute() {
	switch (relations.TYPE) {
	case Relation::Modifies:
		// Create the Modifies Evaluator and evaluate it
		//return ModifiesEvaluator({}, relations, pkb).evaluate();
		auto x = ModifiesEvaluator({}, relations, pkb);
		return x.evaluate();
		//std::unordered_set<std::string> result = { "yyyy" };
		//return result;
	}
}

// TODO: Pattern execute
