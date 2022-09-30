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


	if (isLeftSynonym && isRightSynonym) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = leftSynonymRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(leftSynonym, rightSynonym, result);
		}
	}

	else if (isLeftSynonym && rightArg.isUnderscore()) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	else if (isLeftSynonym && rightArg.isString()) {
		std::unordered_set<std::string> result = leftSynonymRightSimple(rightArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	else if (leftArg.isString() && isRightSynonym) {
		std::unordered_set<std::string> result = leftSimpleRightSynonym(leftArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(rightSynonym, result);
		}
	}
	
	else if (leftArg.isString() && rightArg.isUnderscore()) {
		bool result = leftSimpleRightUnderscore(leftArg.value);
		return result;
	}

	else if (leftArg.isString() && rightArg.isString()) {
		bool result = leftSimpleRightSimple(leftArg.value, rightArg.value);
		return result;
	}

	else if (leftArg.isUnderscore() && isRightSynonym) {
		std::unordered_set<std::string> result = leftUnderscoreRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(rightSynonym, result);
		}
	}

	else if (leftArg.isUnderscore() && rightArg.isString()) {
		bool result = leftUnderscoreRightSimple(rightArg.value);
		return result;
	}

	else if (leftArg.isUnderscore() && rightArg.isUnderscore()) {
		bool result = leftUnderscoreRightUnderScore();
		return result;
	}	
}

