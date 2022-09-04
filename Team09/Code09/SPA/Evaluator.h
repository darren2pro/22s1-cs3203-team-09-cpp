#pragma once
#include <string>
#include <vector>
#include "Relation.h"

class Evaluator {
private:
	std::vector<std::string> declarations;
	Relation relations;
	std::string LEFT_ARG;
	std::string RIGHT_ARG;

public:
	Evaluator(std::vector<std::string> declarations, Relation relations, PKB pkb) : // Added PKB
		declarations(declarations),
		relations(relations),
		LEFT_ARG(relations.LEFT_ARG),
		RIGHT_ARG(relations.RIGHT_ARG) {};

	std::vector<std::string> evaluate();
	virtual std::vector<std::string> leftSynonymRightString(std::string LEFT_ARG, std::string RIGHT_ARG) = 0; // For Demo in Week 5
};
