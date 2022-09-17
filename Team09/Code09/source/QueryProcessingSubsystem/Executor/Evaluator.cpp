#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "Evaluator.h"
#include "../Utils.h"

bool Evaluator::evaluate() {
	if (Utils().isSynonym(LEFT_ARG) && Utils().isSynonym(RIGHT_ARG)) {
		std::unordered_set<std::pair<std::string, std::string>> result = leftSynonymRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(LEFT_ARG, RIGHT_ARG, result);
		}
	}

	else if (Utils().isSynonym(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(LEFT_ARG, result);
		}
	}

	else if (Utils().isSynonym(LEFT_ARG) && (!Utils().isUnderscore(RIGHT_ARG) && !Utils().isSynonym(RIGHT_ARG))) {
		std::unordered_set<std::string> result = leftSynonymRightSimple(RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(LEFT_ARG, result);
		}
	}

	else if ((!Utils().isUnderscore(LEFT_ARG) && !Utils().isSynonym(LEFT_ARG)) && Utils().isSynonym(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSimpleRightSynonym(LEFT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(RIGHT_ARG, result);
		}
	}

	else if ((!Utils().isUnderscore(LEFT_ARG) && !Utils().isSynonym(LEFT_ARG)) && Utils().isUnderscore(RIGHT_ARG)) {
		bool result = leftSimpleRightUnderscore(LEFT_ARG);
		return result;
	}

	else if ((!Utils().isUnderscore(LEFT_ARG) && !Utils().isSynonym(LEFT_ARG)) && (!Utils().isUnderscore(RIGHT_ARG) && !Utils().isSynonym(RIGHT_ARG))) {
		bool result = leftSimpleRightSimple(LEFT_ARG, RIGHT_ARG);
		return result;
	}

	else if (Utils().isUnderscore(LEFT_ARG) && Utils().isSynonym(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftUnderscoreRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(RIGHT_ARG, result);
		}
	}

	else if (Utils().isUnderscore(LEFT_ARG) && (!Utils().isUnderscore(RIGHT_ARG) && !Utils().isSynonym(RIGHT_ARG))) {
		bool result = leftUnderscoreRightSimple(RIGHT_ARG);
		return result;
	}

	else if (Utils().isUnderscore(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		bool result = leftUnderscoreRightUnderScore();
		return result;
	}	
}
