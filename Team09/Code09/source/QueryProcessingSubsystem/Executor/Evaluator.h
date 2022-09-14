#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../Relation.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"

class Evaluator {
private:

public:
	PKB::PKBStorage pkb;
	std::vector<std::string> declarations;
	Relation relations;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

	Evaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) : // Added PKB
		declarations(declarations),
		relations(relations),
		LEFT_ARG(relations.LEFT_ARG),
		RIGHT_ARG(relations.RIGHT_ARG),
		pkb(pkb) {};

	std::unordered_set<std::string> evaluate();

	// Different
	virtual std::unordered_set<std::string> leftSynonymRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::string> leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) = 0;
	virtual std::unordered_set<std::string> leftSynonymRightUnderscore(std::string LEFT_ARG) = 0;

	virtual std::unordered_set<std::string> leftSimpleRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
	virtual std::unordered_set<std::string> leftSimpleRightUnderscore(std::string LEFT_ARG) = 0;
	virtual std::unordered_set<std::string> leftSimpleRightSimple(std::string LEFT_ARG, std::string RIGHT_ARG) = 0;

	virtual std::unordered_set<std::string> leftUnderscoreRightSynonym(std::string RIGHT_ARG) = 0;
	virtual std::unordered_set<std::string> leftUnderscoreRightSimple(std::string RIGHT_ARG) = 0;
	virtual std::unordered_set<std::string> leftUnderscoreRightUnderScore() = 0;
};
