#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class FollowsStarEvaluator : public Evaluator {
public:
	FollowsStarEvaluator(std::vector<std::string> declarations, Relation relations, PKB pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

};
