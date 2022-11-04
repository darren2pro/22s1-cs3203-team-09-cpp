#include <cassert>
#include "AffectsTRelationEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool AffectsTRelationEvaluator::evaluate() {
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

		if (!pkb->isCacheFullyComputed(Relation::AffectsT)) {
			computeFully();
		}

		result = pkb->getRelationSet(Relation::AffectsT);

		if (result.size() == 0) {
			return false;
		}
		else {
			return rdb.insertPairList(leftSynonym, rightSynonym, result);
		}
	}

	// SYNONYM UNDERSCORE
	else if (isLeftSynonym && isRightUnderscore) {
		std::unordered_set<std::string> result = pkb->getRelationAllFirst(Relation::Affects);
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
		if (!pkb->relationContainsSecond(Relation::Affects, rightArg.value)) {
			return false;
		}

		if (!pkb->relationContainsSecond(Relation::AffectsT, rightArg.value)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;
			list.push_back(rightArg.value);
			while (!list.empty()) {
				std::string curr = list.back();
				visited.insert(curr);
				list.pop_back();
				for (const auto& prev : pkb->getRelationFirstFromSecond(Relation::Affects, curr)) {
					pkb->storeCacheSecondToFirstMap(Relation::AffectsT, rightArg.value, prev);
					if (visited.find(prev) == visited.end() && pkb->relationContainsSecond(Relation::Affects, prev)) {
						list.push_back(prev);
					}
				}
			}
		}
		result = pkb->getRelationFirstFromSecond(Relation::AffectsT, rightArg.value);

		return rdb.insertList(leftSynonym, result);
	}

	// SIMPLE SYNONYM
	else if (isLeftSimple && isRightSynonym) {
		std::unordered_set<std::string> result;
		if (!pkb->relationContainsFirst(Relation::Affects, leftArg.value)) {
			return false;
		}

		if (!pkb->relationContainsFirst(Relation::AffectsT, leftArg.value)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;
			list.push_back(leftArg.value);
			while (!list.empty()) {
				std::string curr = list.back();
				visited.insert(curr);
				list.pop_back();
				for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Affects, curr)) {
					pkb->storeCacheFirstToSecondMap(Relation::AffectsT, leftArg.value, next);
					if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Affects, next)) {
						list.push_back(next);
					}
				}
			}
		}
		result = pkb->getRelationSecondFromFirst(Relation::AffectsT, leftArg.value);

		return rdb.insertList(rightSynonym, result);
	}

	// SIMPLE UNDERSCORE
	else if (isLeftSimple && isRightUnderscore) {
		bool result = pkb->relationContainsFirst(Relation::Affects, leftArg.value);
		return result;
	}

	// SIMPLE SIMPLE
	else if (isLeftSimple && isRightSimple) {
		if (!pkb->relationContainsFirst(Relation::Affects, leftArg.value)) {
			return false;
		}

		if (!pkb->relationContainsSet(Relation::AffectsT, leftArg.value, rightArg.value)) {
			std::unordered_set<std::string> visited;
			std::vector<std::string> list;
			list.push_back(leftArg.value);
			while (!list.empty()) {
				std::string curr = list.back();
				visited.insert(curr);
				list.pop_back();
				for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Affects, curr)) {
					pkb->storeCacheSet(Relation::AffectsT, leftArg.value, next);
					if (next == rightArg.value) {
						return true;
					}
					if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Affects, next)) {
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
			std::unordered_set<std::string> result = pkb->getRelationAllSecond(Relation::Affects);
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
			bool result = pkb->relationContainsSecond(Relation::Affects, rightArg.value);
			return result;
		}
		assert("Syntax Error");
	}

	// UNDERSCORE UNDERSCORE
	else if (isLeftUnderscore && isRightUnderscore) {
		if (isFirstArgumentUnderscoreValid(relType)) {
			bool result = !(pkb->relationIsEmpty(Relation::Affects));
			return result;
		}
		assert("Syntax Error");
	}
}

void AffectsTRelationEvaluator::computeFully() {
	if (!pkb->isCacheFullyComputed(Relation::AffectsT)) {
        for (const auto& prev : pkb->getRelationAllFirst(Relation::Affects)) {
		    std::unordered_set<std::string> visited;
		    std::vector<std::string> list;

		    list.push_back(prev);
		    while (!list.empty()) {
			    std::string curr = list.back();
			    visited.insert(curr);
			    list.pop_back();
			    for (const auto& next : pkb->getRelationSecondFromFirst(Relation::Affects, curr)) {
				    pkb->storeRelations(Relation::AffectsT, prev, next);
				    if (visited.find(next) == visited.end() && pkb->relationContainsFirst(Relation::Affects, next)) {
					    list.push_back(next);
				    }
			    }
		    }
	    }
	    pkb->setCacheFullyComputed(Relation::AffectsT);
	}
}