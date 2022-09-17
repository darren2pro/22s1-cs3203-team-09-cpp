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
#include "AssignPatternEvaluator.h"

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	pattern = query->patterns;
	declarations = query->declarations;
	target = query->target;
	ResultsDatabase rdb;

	// Relations clause
	bool relClauseResult = execute(relations, rdb);
	if (!relClauseResult) {
		return { "Error occurred with relation" };
	}

	// Patterns clause
	bool patClauseResult = execute(pattern, rdb);
	if (!patClauseResult) {
		return { "Error occurred with pattern" };
	}

	// IMPLEMENT WITH RUIYAN
	fillRDBWithVariables(declarations, rdb);

	std::unordered_set<std::string> results = getResultsFromRDB(declarations, target, rdb);

	return results;
}


// Relation execute
bool QueryExecutor::execute(Relation relations, ResultsDatabase& rdb) {

	switch (relations.TYPE) {
	case Relation::Modifies:
		return ModifiesEvaluator({}, relations, rdb, pkb).evaluate();
	case Relation::Uses:
		return UsesEvaluator({}, relations, rdb, pkb).evaluate();
	case Relation::Follows:
		return FollowsEvaluator({}, relations, rdb, pkb).evaluate();
	case Relation::FollowsT:
		return FollowsTEvaluator({}, relations, rdb, pkb).evaluate();
	case Relation::Parent:
		return ParentEvaluator({}, relations, rdb, pkb).evaluate();
	case Relation::ParentT:
		return ParentTEvaluator({}, relations, rdb, pkb).evaluate();
	}
}

// Pattern execute
bool QueryExecutor::execute(Pattern pattern, ResultsDatabase& rdb) {

	switch (pattern.TYPE) {
	case Pattern::Assign:
		return AssignPatternEvaluator({}, pattern, rdb, pkb).evaluate();
	}
}

std::unordered_set<std::string> QueryExecutor::getResultsFromRDB(std::vector<Declaration> declarations, Declaration target, ResultsDatabase& rdb) {
	// Different from JK
	return rdb.getResults(target);
}

void QueryExecutor::fillRDBWithVariables(std::vector<Declaration> declarations, ResultsDatabase& rdb) {
	for (Declaration decl : declarations) {
		std::unordered_set<std::string> resultsFromPKB = pkb.getAllVariableAssignment(decl.name);
		rdb.insertList(decl.name, resultsFromPKB);
	}
}

