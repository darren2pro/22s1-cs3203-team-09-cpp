#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ParentEvaluator : public Evaluator {
public:
	ParentEvaluator(std::vector<std::string> declarations, Relation relations, PKB pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor
};
