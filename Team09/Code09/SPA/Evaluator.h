#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "Relation.h"
#include "../source/ProgramKnowledgeBase/PKBStorage.h"

class Evaluator {
private:
	std::vector<std::string> declarations;
	Relation relations;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

public:
	PKB::PKBStorage pkb;

	Evaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) : // Added PKB
		declarations(declarations),
		relations(relations),
		LEFT_ARG(relations.LEFT_ARG),
		RIGHT_ARG(relations.RIGHT_ARG),
		pkb(pkb) {};

	std::unordered_set<std::string> evaluate();
	virtual std::unordered_set<std::string> leftSynonymRightString(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
};
