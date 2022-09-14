#include <string>
#include <vector>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "UsesEvaluator.h"
#include "ModifiesEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentTEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsTEvaluator.h"
#include "PatternEvaluator.h"

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	pattern = query->patterns;
	declarations = query->declarations;
	std::string tokens = query->target;
	std::unordered_set<std::string> results;

	results = execute();
	return results;
}

// Relation execute
std::unordered_set<std::string> QueryExecutor::execute() {

	switch (relations.TYPE) {
	case Relation::Modifies:
		// Create the Modifies Evaluator and evaluate it
		return ModifiesEvaluator({}, relations, pkb).evaluate();
	case Relation::Uses:
		return UsesEvaluator({}, relations, pkb).evaluate();
	case Relation::Follows:
		return FollowsEvaluator({}, relations, pkb).evaluate();
	case Relation::FollowsT:
		return FollowsTEvaluator({}, relations, pkb).evaluate();
	case Relation::Parent:
		return ParentEvaluator({}, relations, pkb).evaluate();
	case Relation::ParentT:
		return ParentTEvaluator({}, relations, pkb).evaluate();
	}

}

// TODO: Pattern execute
//std::unordered_set<std::string> QueryExecutor::execute() {
//
//	switch (pattern.TYPE) {
//		case 
//	}
//
//	//std::unordered_set<std::string> results = {};
//	//return results;
//}
