#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../../Relation.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../../../ProgramKnowledgeBase/PKBStorage.h"
#include "../ResultsDatabase/ResultsDatabase.h"

using namespace PKB;

//! This evaluator is the super class for all of the relations evaluators such as Modifies, Follows.
class RelationEvaluator {
public:
	std::string leftSynonym;
	std::string rightSynonym;

	ResultsDatabase& rdb;
	PKBStorage* pkb;
	std::vector<Declaration> declarations;
	Relation relations;
	Reference leftArg;
	Reference rightArg;

	RelationEvaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBStorage* pkb) : // Added PKB
		declarations(declarations),
		relations(relations),
		leftArg(relations.LEFT_ARG),
		rightArg(relations.RIGHT_ARG),
		rdb(rdb),
		pkb(pkb) {};

    /**
     * Evaluates the relation and stores the results in the ResultsDatabase. Returns false if there are no possible results
     * for this relation evaluation, and returns true if there are at least one possible result.
     */
	bool evaluate();

    //! Strips the quotation marks from the string. (only if there are quotation marks to strip)
	std::string stripQuotationMarks(std::string arg);

	// Check if query is invalid for Uses/Modifies. 
	bool isFirstArgumentUnderscoreValid(enum Relation::Types relType);
};
