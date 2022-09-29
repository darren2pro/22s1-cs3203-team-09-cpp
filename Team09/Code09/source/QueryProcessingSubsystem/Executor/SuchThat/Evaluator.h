#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../../Relation.h"
#include "../../../ProgramKnowledgeBase/PKBManager.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../ResultsDatabase/ResultsDatabase.h"

using namespace PKB;

//! This evaluator is the super class for all of the relations evaluators such as Modifies, Follows.
class Evaluator {
private:
	std::string LEFT_SYNONYM;
	std::string RIGHT_SYNONYM;

public:
	ResultsDatabase& rdb;
	PKBManager* pkb;
	std::vector<Declaration> declarations;
	Relation relations;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

	Evaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager* pkb) : // Added PKB
		declarations(declarations),
		relations(relations),
		LEFT_ARG(relations.LEFT_ARG),
		RIGHT_ARG(relations.RIGHT_ARG),
		rdb(rdb),
		pkb(pkb) {};

    /**
     * Evaluates the relation and stores the results in the ResultsDatabase. Returns false if there are no possible results
     * for this relation evaluation, and returns true if there are at least one possible result.
     */
	bool evaluate();

    //! Strips the quotation marks from the string. (only if there are quotation marks to strip)
	std::string stripQuotationMarks(std::string arg);

	// Different
	virtual std::unordered_set<std::string> leftSynonymRightSimple(std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::pair<std::string, std::string>, pairHash> leftSynonymRightSynonym() = 0;
	virtual std::unordered_set<std::string> leftSynonymRightUnderscore() = 0;

	virtual std::unordered_set<std::string> leftSimpleRightSynonym(std::string LEFT_ARG) = 0; // For Demo in Week 5
	virtual bool leftSimpleRightUnderscore(std::string LEFT_ARG) = 0;
	virtual bool leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) = 0;

	virtual std::unordered_set<std::string> leftUnderscoreRightSynonym() = 0;
	virtual bool leftUnderscoreRightSimple(std::string RIGHT_ARG) = 0;
	virtual bool leftUnderscoreRightUnderScore() = 0;
};
