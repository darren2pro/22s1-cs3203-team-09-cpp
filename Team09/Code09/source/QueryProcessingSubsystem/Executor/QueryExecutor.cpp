#include <string>
#include <vector>
#include <variant>
#include "QueryExecutor.h"
#include "../Relation.h"
#include "../Pattern.h"
#include "../Utils.h"
#include "SuchThat/UsesSEvaluator.h"
#include "Suchthat/UsesPEvaluator.h"
#include "SuchThat/ModifiesSEvaluator.h"
#include "Suchthat/ModifiesPEvaluator.h"
#include "SuchThat/ParentEvaluator.h"
#include "SuchThat/ParentTEvaluator.h"
#include "SuchThat/FollowsEvaluator.h"
#include "SuchThat/FollowsTEvaluator.h"
#include "Suchthat/CallsTEvaluator.h"
#include "Suchthat/CallsEvaluator.h"
#include "Pattern/PatternEvaluator.h"
#include "Pattern/AssignPatternEvaluator.h"
#include "ResultsDatabase/ResultsDatabase.h"


template <class... Ts>
struct Overload : Ts... {
	using Ts::operator()...;
};

template<class... Ts> Overload(Ts...) -> Overload<Ts...>;

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
    bool relClauseResult = true;
    for (Relation& rel : relations) {
        if (!execute(rel, rdb)) {
            relClauseResult = false;
            break;
        }
    }

	// Patterns clause
    bool patClauseResult = true;
    for (Pattern& pat : pattern) {
        if (!execute(pat, rdb)) {
            patClauseResult = false;
            break;
        }
    }

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
	case Relation::ModifiesS:
		return ModifiesSEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::ModifiesP:
		return ModifiesPEvaluator(declarations, relations, rdb, pkb).evaluate();
	case Relation::UsesS:
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

std::unordered_set<std::string> QueryExecutor::getResultsFromRDB(std::variant<Declaration, AttrReference> target, ResultsDatabase& rdb) {
    //! Given a single target, if it is a declaration then simply call the rdb api
    //! If it is attribute reference, then we need to get the declaration first, then get the respective values before returning
    return std::visit(Overload {
            [&](Declaration decl) -> std::unordered_set<std::string> {
                return rdb.getResults(decl);
            },
            [&](AttrReference attrRef) -> std::unordered_set<std::string> {
                /**
                 * TODO: Need to implement this after I understand what is attribute reference
                Declaration decl = getDeclarationFromAttrRef(attrRef, declarations);
                std::unordered_set<std::string> results = rdb.getSynonymSet(decl);
                std::unordered_set<std::string> filteredResults;
                for (std::string result : results) {
                    std::string attrValue = getAttrValueFromAttrRef(attrRef, result, pkb);
                    filteredResults.insert(attrValue);
                }
                return filteredResults;
                 */
                return std::unordered_set<std::string>();
            }
    }, target);
}

void QueryExecutor::insertSynonymSetIntoRDB(Declaration decl, ResultsDatabase& rdb, PKBStorage* pkb) {
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
	case Declaration::Call:
		resultsFromPKB = pkb->getCallSet();
		break;
	}

	rdb.insertList(decl.name, resultsFromPKB);
}
