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
	virtual std::unordered_set<std::string> leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
};
