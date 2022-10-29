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


	// SYNONYM SYNONYM
	if (isLeftSynonym && isRightSynonym) {
		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> result;

		if (!pkb->isCacheFullyComputed(Relation::Affects)) {
			for (const auto& modifiesLine : pkb->getEntitySet(Declaration::Assignment)) {
				for (const auto& usesLine : pkb->getEntitySet(Declaration::Assignment)) {
					// variable modified by modifiesLine
					auto& var = *(pkb->getRelationSecondFromFirst(Relation::ModifiesS, modifiesLine).begin());
					if (!pkb->relationContainsSet(Relation::UsesS, usesLine, var)) {
						continue;
					}
					else { // modifiesLine modifies a variable used in usesLine

					}
				}
			}
			pkb->setCacheFullyComputed(Relation::Affects);
		}

		result = pkb->getRelationSet(Relation::Affects);

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

		if (!pkb->relationContainsSecond(Relation::NextT, rightArg.value)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;
			list.push_back(rightArg.value);
			while (!list.empty()) {
				std::string curr = list.back();
				visited.insert(curr);
				list.pop_back();
				for (const auto& prev : pkb->getRelationFirstFromSecond(Relation::Next, curr)) {
					pkb->storeCacheSecondToFirstMap(Relation::NextT, rightArg.value, prev);
					if (visited.find(prev) == visited.end() && pkb->relationContainsSecond(Relation::Next, prev)) {
						list.push_back(prev);
					}
				}
			}
		}
		result = pkb->getRelationFirstFromSecond(Relation::NextT, rightArg.value);

		return rdb.insertList(leftSynonym, result);
	}

	// SIMPLE SYNONYM
	else if (isLeftSimple && isRightSynonym) {
		std::unordered_set<std::string> result;
		if (!pkb->relationContainsFirst(Relation::Next, leftArg.value)) {
			return false;
		}

		if (!pkb->relationContainsFirst(Relation::NextT, leftArg.value)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;
			list.push_back(leftArg.value);
			while (!list.empty()) {
				std::string curr = list.back();
				visited.insert(curr);
				list.pop_back();
				for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Next, curr)) {
					pkb->storeCacheFirstToSecondMap(Relation::NextT, leftArg.value, next);
					if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Next, next)) {
						list.push_back(next);
					}
				}
			}
		}
		result = pkb->getRelationSecondFromFirst(Relation::NextT, leftArg.value);

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

		if (!pkb->relationContainsSet(Relation::NextT, leftArg.value, rightArg.value)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;
			list.push_back(leftArg.value);
			while (!list.empty()) {
				std::string curr = list.back();
				visited.insert(curr);
				list.pop_back();
				for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Next, curr)) {
					pkb->storeCacheSet(Relation::NextT, leftArg.value, next);
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
		return true;
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