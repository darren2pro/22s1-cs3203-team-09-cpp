#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class ParentEvaluator : public Evaluator {
public:
	ParentEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
