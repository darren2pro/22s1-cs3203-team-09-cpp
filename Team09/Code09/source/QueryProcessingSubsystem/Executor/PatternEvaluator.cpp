#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "QueryExecutor.h"
#include "../Utils.h"

bool PatternEvaluator::evaluate() {
	// Check the left and right argument. If they are synonyms, must
	// get their entire variable set from pkb and populate it first.

	// Pattern synonym is "a" -> pattern a(...) 
	bool isPatternSynonym = Utils().isSynonym(PATTERN_SYNONYM, declarations);
	bool isLeftSynonym = Utils().isSynonym(LEFT_ARG, declarations);

	// Right is definitely not a synonym. It is a matcher

	if (isPatternSynonym) {
		Declaration synonym = Utils().getSynonym(PATTERN_SYNONYM, declarations);
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	if (isLeftSynonym) {
		Declaration synonym = Utils().getSynonym(LEFT_ARG, declarations);
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
		// When inserting, assigned it the wrong index.
	}

	// left underscore
	if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		std::unordered_set<std::string> result = patternLeftUnderscoreRightUnderScore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(PATTERN_SYNONYM, result);
		}
	}
	else if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		pattern.RIGHT_ARG = temporaryStrip(pattern.RIGHT_ARG);
		std::unordered_set<std::string> result = patternLeftUnderscoreRightStrictExpression(pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(PATTERN_SYNONYM, result);
		}
	}
	else if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		pattern.RIGHT_ARG = temporaryStrip(pattern.RIGHT_ARG);
		std::unordered_set<std::string> result = patternLeftUnderscoreRightRelaxedExpression(pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(PATTERN_SYNONYM, result);
		}
	}

	// left synonym
	else if(isLeftSynonym && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> result = patternLeftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(PATTERN_SYNONYM, pattern.LEFT_ARG, result);
		}
	}
	else if (isLeftSynonym && Utils().isStrictExpression(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		pattern.RIGHT_ARG = temporaryStrip(pattern.RIGHT_ARG);
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> result = patternLeftSynonymRightStrictExpression(pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(PATTERN_SYNONYM, pattern.LEFT_ARG, result);
		}
	}
	else if (isLeftSynonym && Utils().isRelaxedExpression(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		pattern.RIGHT_ARG = temporaryStrip(pattern.RIGHT_ARG);
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> result = patternLeftSynonymRightRelaxedExpression(pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(PATTERN_SYNONYM, pattern.LEFT_ARG, result);
		}
	}

	// left simple
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		std::unordered_set<std::string> result = patternLeftSimpleRightUnderscore(pattern.LEFT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(PATTERN_SYNONYM, result);
		}
	}
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		pattern.RIGHT_ARG = temporaryStrip(pattern.RIGHT_ARG);
		std::unordered_set<std::string> result = patternLeftSimpleRightStrictExpression(pattern.LEFT_ARG, pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(PATTERN_SYNONYM, result);
		}
	}
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.RIGHT_ARG)) {
		pattern.LEFT_ARG = temporaryStrip(pattern.LEFT_ARG);
		pattern.RIGHT_ARG = temporaryStrip(pattern.RIGHT_ARG);
		std::unordered_set<std::string> result = patternLeftSimpleRightRelaxedExpression(pattern.LEFT_ARG, pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(PATTERN_SYNONYM, result);
		}
	}
}

std::string PatternEvaluator::temporaryStrip(std::string arg) {
	// Check whether there is underscores to strip first
	arg.erase(std::remove(arg.begin(), arg.end(), '"'), arg.end());
	return arg;
}

