#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "Evaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool Evaluator::evaluate() {
	// Check the left and right argument. If they are synonyms, must
	// get their entire variable set from pkb and populate it first.
	bool isLeftSynonym = leftArg.isSynonym();
	bool isRightSynonym = rightArg.isSynonym();
	bool isLeftSimple = leftArg.isString() || leftArg.isStmtNum();
	bool isRightSimple = rightArg.isString() || rightArg.isStmtNum();
	bool isLeftUnderscore = leftArg.isUnderscore();
	bool isRightUnderscore = rightArg.isUnderscore();

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
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = leftSynonymRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(leftSynonym, rightSynonym, result);
		}
	}

	// SYNONYM UNDERSCORE
	else if (isLeftSynonym && isRightUnderscore) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	// SYNONYM SIMPLE
	else if (isLeftSynonym && isRightSimple) {
		std::unordered_set<std::string> result = leftSynonymRightSimple(rightArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	// SIMPLE SYNONYM
	else if (isLeftSimple && isRightSynonym) {
		std::unordered_set<std::string> result = leftSimpleRightSynonym(leftArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(rightSynonym, result);
		}
	}
	
	// SIMPLE UNDERSCORE
	else if (isLeftSimple && isRightUnderscore) {
		bool result = leftSimpleRightUnderscore(leftArg.value);
		return result;
	}

	// SIMPLE SIMPLE
	else if ((leftArg.isString() || leftArg.isStmtNum()) && (rightArg.isString() || rightArg.isStmtNum())) {
		bool result = leftSimpleRightSimple(leftArg.value, rightArg.value);
		return result;
	}

	// UNDERSCORE SYNONYM
	else if (isLeftUnderscore && isRightSynonym) {
		std::unordered_set<std::string> result = leftUnderscoreRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(rightSynonym, result);
		}
	}

	// UNDERSCORE SIMPLE
	else if (isLeftUnderscore && isRightSimple) {
		bool result = leftUnderscoreRightSimple(rightArg.value);
		return result;
	}

	// UNDERSCORE UNDERSCORE
	else if (isLeftUnderscore && isRightUnderscore) {
		bool result = leftUnderscoreRightUnderScore();
		return result;
	}	
}

