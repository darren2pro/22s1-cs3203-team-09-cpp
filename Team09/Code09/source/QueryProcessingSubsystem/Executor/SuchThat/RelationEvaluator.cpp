#include <cassert>
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "RelationEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool RelationEvaluator::evaluate() {
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
	else if ((leftArg.isString() || leftArg.isStmtNum()) && (rightArg.isString() || rightArg.isStmtNum())) {
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

bool RelationEvaluator::isFirstArgumentUnderscoreValid(enum Relation::Types relType) {
	if(relType == Relation::UsesP || relType == Relation::UsesS || relType == Relation::ModifiesP || relType == Relation::ModifiesS) {
		return false;
	}
	return true;
}

