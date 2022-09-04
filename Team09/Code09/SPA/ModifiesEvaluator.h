#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ModifiesEvaluator : public Evaluator {
public:
	ModifiesEvaluator(std::vector<std::string> declarations, Relation relations, PKB pkb) :
	Evaluator(declarations, relations, pkb) {}; // Constructor

	std::vector<std::string> ModifiesEvaluator::leftSynonymRightString(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Example: such that Modify(a, "x")

		// Call PKB API here.
		std::vector<std::string> results = pkb.getModifyBy(rightarg);
		results = filter(LEFT_ARG, results);
		return {};
	};
};
