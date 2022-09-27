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
	bool isLeftSynonym = Utils().isSynonym(LEFT_ARG, declarations);
	bool isRightSynonym = Utils().isSynonym(RIGHT_ARG, declarations);

	if (isLeftSynonym) {
		Declaration synonym = Utils().getSynonym(LEFT_ARG, declarations);
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	if (isRightSynonym) {
		Declaration synonym = Utils().getSynonym(RIGHT_ARG, declarations);
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}


	if (isLeftSynonym && isRightSynonym) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result = leftSynonymRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(LEFT_ARG, RIGHT_ARG, result);
		}
	}

	else if (isLeftSynonym && Utils().isUnderscore(RIGHT_ARG)) {
		std::unordered_set<std::string> result = leftSynonymRightUnderscore();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(LEFT_ARG, result);
		}
	}

	else if (isLeftSynonym && Utils().isBasicQuerySimple(RIGHT_ARG)) {
		RIGHT_ARG = stripQuotationMarks(RIGHT_ARG);
		std::unordered_set<std::string> result = leftSynonymRightSimple(RIGHT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(LEFT_ARG, result);
		}
	}

	else if (Utils().isBasicQuerySimple(LEFT_ARG) && isRightSynonym) {
		std::unordered_set<std::string> result = leftSimpleRightSynonym(LEFT_ARG);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(RIGHT_ARG, result);
		}
	}
	
	else if (Utils().isBasicQuerySimple(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		bool result = leftSimpleRightUnderscore(LEFT_ARG);
		return result;
	}

	else if (Utils().isBasicQuerySimple(LEFT_ARG) && Utils().isBasicQuerySimple(RIGHT_ARG)) {
		RIGHT_ARG = stripQuotationMarks(RIGHT_ARG);
		bool result = leftSimpleRightSimple(LEFT_ARG, RIGHT_ARG);
		return result;
	}

	else if (Utils().isUnderscore(LEFT_ARG) && isRightSynonym) {
		std::unordered_set<std::string> result = leftUnderscoreRightSynonym();
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(RIGHT_ARG, result);
		}
	}

	else if (Utils().isUnderscore(LEFT_ARG) && Utils().isBasicQuerySimple(RIGHT_ARG)) {
		RIGHT_ARG = stripQuotationMarks(RIGHT_ARG);
		bool result = leftUnderscoreRightSimple(RIGHT_ARG);
		return result;
	}

	else if (Utils().isUnderscore(LEFT_ARG) && Utils().isUnderscore(RIGHT_ARG)) {
		bool result = leftUnderscoreRightUnderScore();
		return result;
	}	
}

std::string Evaluator::stripQuotationMarks(std::string arg) {
	//! Check whether there is underscores to strip first
	if (arg[0] == '"' && arg[arg.length() - 1] == '"') {
		arg.erase(0, 1);
		arg.erase(arg.length() - 1, 1);
	}
	return arg;
}
