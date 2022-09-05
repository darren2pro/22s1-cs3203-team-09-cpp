#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ParentStarEvaluator : public Evaluator {
public:
	ParentStarEvaluator(std::vector<std::string> declarations, Relation relations, PKB pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor
};
