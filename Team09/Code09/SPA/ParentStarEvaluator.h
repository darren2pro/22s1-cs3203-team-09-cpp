#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ParentStarEvaluator : public Evaluator {
public:
	ParentStarEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
