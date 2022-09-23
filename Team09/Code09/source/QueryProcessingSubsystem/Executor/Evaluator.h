#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../Relation.h"
#include "../../ProgramKnowledgeBase/PKBManager.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include "ResultsDatabase/ResultsDatabase.h"

using namespace PKB;

class Evaluator {
private:
	std::string LEFT_SYNONYM;
	std::string RIGHT_SYNONYM;

public:
	ResultsDatabase& rdb;
	PKBManager pkb;
	std::vector<Declaration> declarations;
	Relation relations;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

	Evaluator(std::vector<Declaration> declarations, Relation relations, ResultsDatabase& rdb, PKBManager pkb) : // Added PKB
		declarations(declarations),
		relations(relations),
		LEFT_ARG(relations.LEFT_ARG),
		RIGHT_ARG(relations.RIGHT_ARG),
		rdb(rdb),
		pkb(pkb) {};

	bool evaluate();
	std::string temporaryStrip(std::string arg);

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
