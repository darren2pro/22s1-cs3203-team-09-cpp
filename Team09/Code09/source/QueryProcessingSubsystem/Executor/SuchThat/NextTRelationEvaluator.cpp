#include <cassert>
#include "NextTRelationEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool NextTRelationEvaluator::evaluate() {
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
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result;

		for (const auto& prev : pkb->getRelationAllFirst(Relation::Next)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;

			list.push_back(prev);
            while (!list.empty()) {
                std::string curr = list.back();
                visited.insert(curr);
                list.pop_back();
                for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Next, curr)) {
                    result.insert(std::make_pair(prev, next));
                    if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Next, next)) {
                        list.push_back(next);
                    }
                }
            }
        }

		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(leftSynonym, rightSynonym, result);
		}
	}

	// SYNONYM UNDERSCORE
	else if (isLeftSynonym && isRightUnderscore) {
		std::unordered_set<std::string> result = pkb->getRelationAllFirst(Relation::Next);
		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertList(leftSynonym, result);
		}
	}

	// SYNONYM SIMPLE
	else if (isLeftSynonym && isRightSimple) {
		std::unordered_set<std::string> result;
		if (!pkb->relationContainsSecond(Relation::Next, rightArg.value)) {
			return false;
		}
		std::unordered_set<std::string> visited;
		std::vector<std::string> list;
		list.push_back(rightArg.value);
		while (!list.empty()) {
			std::string curr = list.back();
			visited.insert(curr);
			list.pop_back();
			for (const auto& prev : pkb->getRelationFirstFromSecond(Relation::Next, curr)) {
				result.insert(prev);
				if (visited.find(prev) == visited.end() && pkb->relationContainsSecond(Relation::Next, prev)) {
					list.push_back(prev);
				}
			}
		}
		return rdb.insertList(leftSynonym, result);
	}

	// SIMPLE SYNONYM
	else if (isLeftSimple && isRightSynonym) {
		std::unordered_set<std::string> result;
		if (!pkb->relationContainsFirst(Relation::Next, leftArg.value)) {
			return false;
		}
		std::unordered_set<std::string> visited;
		std::vector<std::string> list;
		list.push_back(leftArg.value);
		while (!list.empty()) {
			std::string curr = list.back();
			visited.insert(curr);
			list.pop_back();
			for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Next, curr)) {
				result.insert(next);
				if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Next, next)) {
					list.push_back(next);
				}
			}
		}
	    return rdb.insertList(rightSynonym, result);
	}

	// SIMPLE UNDERSCORE
	else if (isLeftSimple && isRightUnderscore) {
		bool result = pkb->relationContainsFirst(Relation::Next, leftArg.value);
		return result;
	}

	// SIMPLE SIMPLE
	else if (isLeftSimple && isRightSimple) {
		if (!pkb->relationContainsFirst(Relation::Next, leftArg.value)) {
			return false;
		}
		std::unordered_set<std::string> visited;
		std::vector<std::string> list;
        list.push_back(leftArg.value);
		while (!list.empty()) {
			std::string curr = list.back();
			visited.insert(curr);
			list.pop_back();
			for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Next, curr)) {
				if (next == rightArg.value) {
					return true;
				}
				if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Next, next)) {
					list.push_back(next);
				}
			}
		}
		return false;
	}

	// TODO
	// UsesP/S, ModifiesP/S needs to add assert to capture the error.
	// Create another function to check validity?

	// UNDERSCORE SYNONYM
	else if (isLeftUnderscore && isRightSynonym) {
		if (isFirstArgumentUnderscoreValid(relType)) {
			std::unordered_set<std::string> result = pkb->getRelationAllSecond(Relation::Next);
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
			bool result = pkb->relationContainsSecond(Relation::Next, rightArg.value);
			return result;
		}
		assert("Syntax Error");
	}

	// UNDERSCORE UNDERSCORE
	else if (isLeftUnderscore && isRightUnderscore) {
		if (isFirstArgumentUnderscoreValid(relType)) {
			bool result = !(pkb->relationIsEmpty(Relation::Next));
			return result;
		}
		assert("Syntax Error");
	}
}