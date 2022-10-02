#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool PatternEvaluator::evaluate() {
	// Check the left and right argument. If they are synonyms, must
	// get their entire variable set from pkb and populate it first.

	// Pattern synonym is "a" -> pattern a(...) 
	bool isLeftSynonym = leftArg.isSynonym();
	bool isLeftUnderscore = leftArg.isUnderscore();
	bool isLeftSimple = leftArg.isString(); // Will never be a statement number, so only check if its a string.
	bool isRightStrict = rightArg.isStrict();
	bool isRightRelaxed = rightArg.isRelaxed();
	bool isRightUnderscore = rightArg.isUnderscore();

	// Insert the pattern synonym into RDB. "a" -> pattern a(...)
	QueryExecutor::insertSynonymSetIntoRDB(patternArg, rdb, pkb);

	// No need check right. Right will never be synonym.
	if (isLeftSynonym) {
		Declaration synonym = leftArg.declaration;
		leftSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	// left underscore
	if (isLeftUnderscore && isRightUnderscore) {
		std::unordered_set<std::string> result = patternLeftUnderscoreRightUnderScore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(patternSynonym, result);
		}
	}
	else if (isLeftUnderscore && isRightStrict) {
		std::unordered_set<std::string> result = patternLeftUnderscoreRightStrictExpression(rightString);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(patternSynonym, result);
		}
	}
	else if (isLeftUnderscore && isRightRelaxed) {
		std::unordered_set<std::string> result = patternLeftUnderscoreRightRelaxedExpression(rightString);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(patternSynonym, result);
		}
	}

	// left synonym
	else if(isLeftSynonym && isRightUnderscore) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = patternLeftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(patternSynonym, leftSynonym, result);
		}
	}
	else if (isLeftSynonym && isRightStrict) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = patternLeftSynonymRightStrictExpression(rightString);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(patternSynonym, leftSynonym, result);
		}
	}
	else if (isLeftSynonym && isRightRelaxed) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = patternLeftSynonymRightRelaxedExpression(rightString);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(patternSynonym, leftSynonym, result);
		}
	}

	// left simple -> just need to check whether it is a string. It will never be a statement number.
	else if (isLeftSimple && isRightUnderscore) {
		std::unordered_set<std::string> result = patternLeftSimpleRightUnderscore(leftArg.value);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(patternSynonym, result);
		}
	}
	else if (isLeftSimple && isRightStrict) {
		std::unordered_set<std::string> result = patternLeftSimpleRightStrictExpression(leftArg.value, rightString);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(patternSynonym, result);
		}
	}
	else if (isLeftSimple && isRightRelaxed) {
		std::unordered_set<std::string> result = patternLeftSimpleRightRelaxedExpression(leftArg.value, rightString);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(patternSynonym, result);
		}
	}
}

