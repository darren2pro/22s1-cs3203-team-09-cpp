#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../../ProgramKnowledgeBase/PKBStorage.h"
#include <iostream>

using namespace std;

class UsesEvaluator: public Evaluator {
public:
	UsesEvaluator(std::vector<std::string> declarations, Relation relations, PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> UsesEvaluator::leftSynonymRightSynonym(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Example: such that Uses(a, "x")
		// Call PKB API here.
		//std::unordered_set<std::string> results = pkb.getAllUses();
		//return results;
		
		return { "<PKB Uses API Return Result Here>" };
	};
};
