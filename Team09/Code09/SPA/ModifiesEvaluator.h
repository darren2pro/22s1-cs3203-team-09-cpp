#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"
#include "../source/ProgramKnowledgeBase/PKBStorage.h"

class ModifiesEvaluator : public Evaluator {
public:
	ModifiesEvaluator(std::vector<std::string> declarations, Relation relations, PKB::PKBStorage pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::unordered_set<std::string> ModifiesEvaluator::leftSynonymRightString(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Example: such that Modify(a, "x")
		// Call PKB API here.

		std::unordered_set<std::string> results = pkb.getAllModify();
		//std::unordered_set<std::string> results = pkbgetAllModify();
		//results = filter(LEFT_ARG, results);
		return results;
	};
};
