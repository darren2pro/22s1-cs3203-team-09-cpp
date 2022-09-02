#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ModifiesEvaluator : public Evaluator {
public:
	ModifiesEvaluator(std::vector<std::string> declarations, Relation relations) :
	Evaluator(declarations, relations) {}; // Constructor

	std::vector<std::string> ModifiesEvaluator::leftSynonymRightString(std::string LEFT_ARG, std::string RIGHT_ARG) override {
		// Call PKB API here.
		return {};
	};
};
