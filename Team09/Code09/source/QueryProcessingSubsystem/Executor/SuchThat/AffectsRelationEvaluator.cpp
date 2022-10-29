#include <cassert>
#include "AffectsRelationEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool AffectsRelationEvaluator::evaluate() {
	// Check the left and right argument. If they are synonyms, must
	// get their entire variable set from pkb and populate it first.
	bool isLeftSynonym = leftArg.isSynonym();
	bool isRightSynonym = rightArg.isSynonym();
	bool isLeftSimple = leftArg.isString() || leftArg.isStmtNum();
	bool isRightSimple = rightArg.isString() || rightArg.isStmtNum();
	bool isLeftUnderscore = leftArg.isUnderscore();
	bool isRightUnderscore = rightArg.isUnderscore();

	auto relType = relations.Type;

	if (isLeftSynonym) {
		Declaration synonym = leftArg.declaration;
		leftSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	if (isRightSynonym) {
		Declaration synonym = rightArg.declaration;
		rightSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	if (!pkb->isCacheFullyComputed(Relation::Affects)) {
		computeFully();
	}

	// SYNONYM SYNONYM
	if (isLeftSynonym && isRightSynonym) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = pkb->getRelationSet(relType);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(leftSynonym, rightSynonym, result);
		}
	}

	// SYNONYM UNDERSCORE
	else if (isLeftSynonym && isRightUnderscore) {
		std::unordered_set<std::string> result = pkb->getRelationAllFirst(relType);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	// SYNONYM SIMPLE
	else if (isLeftSynonym && isRightSimple) {
		std::unordered_set<std::string> result = pkb->getRelationFirstFromSecond(relType, rightArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	// SIMPLE SYNONYM
	else if (isLeftSimple && isRightSynonym) {
		std::unordered_set<std::string> result = pkb->getRelationSecondFromFirst(relType, leftArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(rightSynonym, result);
		}
	}

	// SIMPLE UNDERSCORE
	else if (isLeftSimple && isRightUnderscore) {
		bool result = pkb->relationContainsFirst(relType, leftArg.value);
		return result;
	}

	// SIMPLE SIMPLE
	else if (isLeftSimple && isRightSimple) {
		bool result = pkb->relationContainsSet(relType, leftArg.value, rightArg.value);
		return result;
	}

	// TODO
	// UsesP/S, ModifiesP/S needs to add assert to capture the error.
	// Create another function to check validity?

	// UNDERSCORE SYNONYM
	else if (isLeftUnderscore && isRightSynonym) {
		if (isFirstArgumentUnderscoreValid(relType)) {
			std::unordered_set<std::string> result = pkb->getRelationAllSecond(relType);
			if (result.size() == 0) {
				return false;
			}
			else {
				return rdb.insertList(rightSynonym, result);
			}
		}
		assert("Syntax Error");
	}

	// UNDERSCORE SIMPLE
	else if (isLeftUnderscore && isRightSimple) {
		if (isFirstArgumentUnderscoreValid(relType)) {
			bool result = pkb->relationContainsSecond(relType, rightArg.value);
			return result;
		}
		assert("Syntax Error");
	}

	// UNDERSCORE UNDERSCORE
	else if (isLeftUnderscore && isRightUnderscore) {
		if (isFirstArgumentUnderscoreValid(relType)) {
			bool result = !(pkb->relationIsEmpty(relType));
			return result;
		}
		assert("Syntax Error");
	}
}

void AffectsRelationEvaluator::computeFully() {
	for (const auto& modifiesLine : pkb->getEntitySet(Declaration::Assignment)) {
		for (const auto& usesLine : pkb->getEntitySet(Declaration::Assignment)) {
			// variable modified by modifiesLine
			for (const auto& var : pkb->getRelationSecondFromFirst(Relation::ModifiesS, modifiesLine)) {
                if (lineUsesVar(usesLine, var) &&
				    lineReachesline(modifiesLine, usesLine) &&
				    isNotModified(modifiesLine, usesLine, var)) {
				    pkb->storeRelations(Relation::Affects, modifiesLine, usesLine);
			    }
			}
		}
	}
	pkb->setCacheFullyComputed(Relation::Affects);
}

bool AffectsRelationEvaluator::lineUsesVar(std::string uses, std::string var) {
	return pkb->relationContainsSet(Relation::UsesS, uses, var);
}

bool AffectsRelationEvaluator::lineReachesline(std::string modifies, std::string uses) {
	return pkb->relationContainsSet(Relation::NextT, modifies, uses);
}

bool AffectsRelationEvaluator::isNotModified(std::string modifies, std::string uses, std::string var) {
	std::unordered_set<std::string> visited;
	std::vector<std::string> list;
	list.push_back(modifies);
	while (!list.empty()) {
		std::string curr = list.back();
		visited.insert(curr);
		list.pop_back();
		for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Next, curr)) {
			if (next == uses) {
				return true;
			}
			if ((pkb->entityContains(Declaration::Assignment, next) ||
				 pkb->entityContains(Declaration::Read, next) ||
				 pkb->entityContains(Declaration::Call, next)) &&
				pkb->relationContainsSet(Relation::ModifiesS, next, var)) {
				break;
			}
			if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Next, next)) {
				list.push_back(next);
			}
		}
	}
	return false;
}