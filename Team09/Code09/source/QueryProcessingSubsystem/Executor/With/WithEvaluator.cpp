#include <string>
#include <vector>
#include <cassert>
#include "WithEvaluator.h"
#include "../QueryExecutor.h"
#include "../../Utils.h"

bool WithEvaluator::evaluate() {
	//// Check the left and right argument. If they are synonyms, must
	//// get their entire variable set from pkb and populate it first.

	bool isLeftInt = leftArg.isInteger();
	bool isLeftIdent = leftArg.isIdent();
	bool isLeftAttrRef = leftArg.isSynonym();

	bool isRightInt = rightArg.isInteger();
	bool isRightIdent = rightArg.isIdent();
	bool isRightAttrRef = rightArg.isSynonym();

	// Pre-fill for the 2 attribute references
	if (isLeftAttrRef) {
		Declaration synonym = leftArg.declaration;
		leftSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	if (isRightAttrRef) {
		Declaration synonym = rightArg.declaration;
		leftSynonym = synonym.name;
		QueryExecutor::insertSynonymSetIntoRDB(synonym, rdb, pkb);
	}

	// NOTE FOR ATTRIBUTE REFERENCE
	// only exceptions: call/read/print.procName -> stmt# to procname. The rest of attrRef is just itself, no change.

	// Int Int
	if (isLeftInt && isRightInt) {
		// With 5 = 5
		return leftArg == rightArg;
	}
	// Ident Ident
	else if (isLeftIdent && isRightIdent) {
		// with "cenX" == "cenX"
		return leftArg == rightArg;
	}
	// AttrRef AttrRef
	else if (isLeftAttrRef && isRightAttrRef) {
		Declaration leftDeclaration = leftArg.declaration;
		auto leftDesignEntityType = leftDeclaration.Type;
		auto leftSynonym = leftDeclaration.name;
		auto leftAttr = leftArg.attr;
		
		Declaration rightDeclaration = rightArg.declaration;
		auto rightDesignEntityType = rightDeclaration.Type;
		auto rightSynonym = rightDeclaration.name;
		auto rightAttr = rightArg.attr;

		// Check if the synonyms are in RDB first.
		std::unordered_set<std::string> leftSynonymValues;
		if (rdb.variableIsPresent(leftSynonym)) {
			leftSynonymValues = rdb.getResults(leftDeclaration);
		}
		else {
			leftSynonymValues = pkb->getEntitySet(leftDeclaration.Type);
		}
		
		std::unordered_set<std::string> rightSynonymValues;
		if (rdb.variableIsPresent(rightSynonym)) {
			rightSynonymValues = rdb.getResults(rightDeclaration);
		}
		else {
			rightSynonymValues = pkb->getEntitySet(rightDeclaration.Type);
		}

		std::unordered_set<std::pair<std::string, std::string>, PKB::pairHash> pairList;
		for (auto& leftVal : leftSynonymValues) {
			for (auto& rightVal : rightSynonymValues) {
				auto leftAttrVal = pkb->getValueFromKey(leftVal, leftDesignEntityType, leftAttr);
				auto rightAttrVal = pkb->getValueFromKey(rightVal, rightDesignEntityType, rightAttr);
				if (leftAttrVal == rightAttrVal) {
					pairList.insert({ leftVal, rightVal });
				}
			}
		}

		return rdb.insertPairList(leftSynonym, rightSynonym, pairList);
	}

	// Int Ident
	else if ((isLeftInt && isRightIdent) || (isRightInt && isLeftIdent))  {
		// Invalid query.
		return false;
	}
	// Int AttrRef
	else if ((isLeftInt && isRightAttrRef) || (isLeftAttrRef && isRightInt)) {
		// With s.stmt# = 5
		// Idea: fetch all lines for s, then if lines contain 5, set s table to be only 5.
		std::string synonym;
		std::unordered_set<std::string> list;
		if (isLeftInt) {
			synonym = rightArg.declaration.name;
			list.insert(leftArg.value);
		}
		else {
			synonym = leftArg.declaration.name;
			list.insert(rightArg.value);
		}

		return rdb.insertList(synonym, list);
	}
	// Ident AttrRef
	else if ((isLeftIdent && isRightAttrRef) || (isLeftAttrRef && isRightIdent)) {
		// With "proc1" = c.procName
		// Check what is the synonym type first. If variable/procedure, instantly add into rdb.

		std::string ident;
		std::string synonym;
		Declaration declaration;
		enum AttrReference::Attribute attribute;
		enum Declaration::DesignEntity entityType;
		std::unordered_set<std::string> list;
		if (isLeftAttrRef) {
			ident = rightArg.value;
			synonym = leftArg.declaration.name;
			declaration = leftArg.declaration;
			attribute = leftArg.attr;
			entityType = leftArg.declaration.Type;
		}
		else {
			ident = leftArg.value;
			synonym = rightArg.declaration.name;
			declaration = rightArg.declaration;
			attribute = rightArg.attr;
			entityType = rightArg.declaration.Type;
		}

		if (entityType == Declaration::DesignEntity::Procedure || entityType == Declaration::DesignEntity::Variable) {
			list.insert(ident);
			return rdb.insertList(synonym, list);
		}
		// Not Variable or Procedure. -> Can only be call.procName, read/print.varName
		// Check whether synonym has prev values. If not, query PKB
		else {
			if (rdb.variableIsPresent(synonym)) {
				list = rdb.getResults(declaration);
			}
			else {
				list = pkb->getEntitySet(declaration.Type);
			}

			// newList contains all the line number of call/read/print that have proc/varName == ident.
			std::unordered_set<std::string> newList;
			for (auto& val : list) {
				auto attributeVal = pkb->getValueFromKey(val, entityType, attribute);
				if (attributeVal == ident) {
					newList.insert(val);
				}
			}

			return rdb.insertList(synonym, newList);
		}
	}
	else {
		assert("Should not enter here.");
	}
}

