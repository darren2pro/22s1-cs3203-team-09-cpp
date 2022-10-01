#include <string>
#include <vector>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Utils.h"
#include "SuchThat/UsesSEvaluator.h"
#include "SuchThat/ModifiesSEvaluator.h"
#include "SuchThat/ParentEvaluator.h"
#include "SuchThat/ParentTEvaluator.h"
#include "SuchThat/FollowsEvaluator.h"
#include "SuchThat/FollowsTEvaluator.h"
#include "Pattern/PatternEvaluator.h"
#include "Pattern/AssignPatternEvaluator.h"
#include "ResultsDatabase/ResultsDatabase.h"
#include "suchthat/CallsTEvaluator.h"
#include "suchthat/CallsEvaluator.h"
#include "suchthat/ModifiesPEvaluator.h"
#include "suchthat/UsesPEvaluator.h"

std::unordered_set<std::string> QueryExecutor::processQuery(Query* query) {
	relations = query->relations;
	pattern = query->patterns;
	declarations = query->declarations;
	target = query->target;
	rdb = ResultsDatabase();

	// auto clauses = prioritizeClauses(query)
	// for clause in clauses:
	// if(!execute(clause)) { return False }

	// Relations clause
	bool relClauseResult = execute(relations, rdb);

	// Patterns clause
	bool patClauseResult = execute(pattern, rdb);

	// Return empty list if any of them returns False.
	if (!relClauseResult || !patClauseResult) {
		return {};
	}

	// Insert all other variables that have not been inserted.
	for (Declaration decl : declarations) {
		insertSynonymSetIntoRDB(decl, rdb, pkb);
	}

	std::unordered_set<std::string> results = getResultsFromRDB(target, rdb);

	return results;
}


// Relation execute
bool QueryExecutor::execute(Relation relations, ResultsDatabase& rdb) {

	switch (relations.TYPE) {
	case Relation::ModifiesS: // ModifiesS
		return ModifiesSEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::ModifiesP:
		return ModifiesPEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::UsesS: // UsesS
		return UsesSEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::UsesP:
		return UsesPEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::Follows:
		return FollowsEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::FollowsT:
		return FollowsTEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::Parent:
		return ParentEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::ParentT:
		return ParentTEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::Calls:
		return CallsEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::CallsT:
		return CallsTEvaluator(declarations, relations, rdb, pkb).evaluate();
	default:
		return true;
	}
}
 
// Pattern execute
bool QueryExecutor::execute(Pattern pattern, ResultsDatabase& rdb) {

	switch (pattern.TYPE) {
	case Pattern::Assign:
		return AssignPatternEvaluator(declarations, pattern, rdb, pkb).evaluate();
	case Pattern::NONE:
		return true;
	default:
		return true;
	}
}

std::unordered_set<std::string> QueryExecutor::getResultsFromRDB(Declaration target, ResultsDatabase& rdb) {
	// Different from JK
	return rdb.getResults(target);
}

void QueryExecutor::insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBManager* pkb) {
	std::unordered_set<std::string> resultsFromPKB;
	
	if (rdb.variableIsPresent(decl.name)) return;

	switch (decl.TYPE) {
	case Declaration::Assignment:
		resultsFromPKB = pkb->getAssignSet();
		break;
	case Declaration::Variable:
		resultsFromPKB = pkb->getVariableSet();
		break;
	case Declaration::Procedure:
		resultsFromPKB = pkb->getProcedureSet();
		break;
	case Declaration::Constant:
		resultsFromPKB = pkb->getConstantSet();
		break;
	case Declaration::While:
		resultsFromPKB = pkb->getWhileSet();
		break;
	case Declaration::If:
		resultsFromPKB = pkb->getIfSet();
		break;
	case Declaration::Read:
		resultsFromPKB = pkb->getReadSet();
		break;
	case Declaration::Print:
		resultsFromPKB = pkb->getPrintSet();
		break;
	case Declaration::Statement:
		resultsFromPKB = pkb->getStmtSet();
		break;
	}

	rdb.insertList(decl.name, resultsFromPKB);
}
