#include <string>
#include <vector>
#include "PatternEvaluator.h"
#include "../Utils.h"

bool PatternEvaluator::evaluate() {

	// left underscore
	if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> result = patternLeftUnderscoreRightUnderScore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(SYNONYM, result);
		}
	}
	else if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> result = patternLeftUnderscoreRightStrictExpression(pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(SYNONYM, result);
		}
	}
	else if (Utils().isUnderscore(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> result = patternLeftUnderscoreRightRelaxedExpression(pattern.RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(SYNONYM, result);
		}
	}

	// left synonym
	else if(Utils().isSynonym(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> result = patternLeftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(SYNONYM, LEFT_ARG, result);
		}
	}
	else if (Utils().isSynonym(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.RIGHT_ARG)) {
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> result = patternLeftSynonymRightStrictExpression(RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(SYNONYM, LEFT_ARG, result);
		}
	}
	else if (Utils().isSynonym(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.RIGHT_ARG)) {
		std::unordered_set<std::pair<std::string, std::string>, PairHasher::pairHash> result = patternLeftSynonymRightRelaxedExpression(RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(SYNONYM, LEFT_ARG, result);
		}
	}

	// left simple
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isUnderscore(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> result = patternLeftSimpleRightUnderscore(LEFT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(SYNONYM, result);
		}
	}
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isStrictExpression(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> result = patternLeftSimpleRightStrictExpression(LEFT_ARG, RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(SYNONYM, result);
		}
	}
	else if (Utils().isString(pattern.LEFT_ARG) && Utils().isRelaxedExpression(pattern.RIGHT_ARG)) {
		std::unordered_set<std::string> result = patternLeftSimpleRightRelaxedExpression(LEFT_ARG, RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(SYNONYM, result);
		}
	}


}
