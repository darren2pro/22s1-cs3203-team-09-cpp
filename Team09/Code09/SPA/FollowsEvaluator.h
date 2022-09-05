#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../source/ProgramKnowledgeBase/PKBStorage.h"

class FollowsEvaluator : public Evaluator {
public:
	FollowsEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	//std::vector<std::string> evaluate() override {
	//	return {};
	//}
};
