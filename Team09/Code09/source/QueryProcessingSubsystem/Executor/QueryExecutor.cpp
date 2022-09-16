#include <string>
#include <vector>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Utils.h"
#include "UsesEvaluator.h"
#include "ModifiesEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentTEvaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsTEvaluator.h"
#include "PatternEvaluator.h"
#include "ResultsDatabase/ResultsDatabase.h"

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	pattern = query->patterns;
	declarations = query->declarations;
	target = query->target;


	 //COMMENTED OUT FOR MILESTONE 1 INTEGRATION TESTING
	/*if (!execute(relations)) {
		return { "Error" };
	}*/
	auto suchThatResults = execute(relations);
	//auto patternResults = execute(patterns);
	
	//// CREATE CONSTRAIN DB
	//ResultsDatabase rdb;

	//// INSERT SUCH THAT RESULTS
	//if (Utils().isList(suchThatResults)) {
	//	// List
	//	std::string assignmentSynonym;
	//	if (Utils().isSynonym(relations.LEFT_ARG)) {
	//		assignmentSynonym = relations.LEFT_ARG;
	//	}
	//	else {
	//		assignmentSynonym = relations.RIGHT_ARG;
	//	}
	//	rdb.insertList(assignmentSynonym, suchThatResults);

	//} else if(Utils().isListPair(suchThatResults)) {
	//	// ListPair 
	//	rdb.insertPairList(relations.LEFT_ARG, relations.RIGHT_ARG, suchThatResults);
	//}
	//else {
	//	// Boolean
	//	rdb.insertBoolean(suchThatResults);
	//}

	//std::unordered_set<std::string> results = rdb.getResults(target);

	std::unordered_set<std::string> results = {};
	return results;
}

// Relation execute
auto QueryExecutor::execute(Relation relation) {

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
//std::unordered_set<std::string> QueryExecutor::execute(Pattern pattern) {
//
//	switch (pattern.TYPE) {
//	case Pattern::Assign:
//		return 
//	}
//
//	//std::unordered_set<std::string> results = {};
//	//return results;
//}

