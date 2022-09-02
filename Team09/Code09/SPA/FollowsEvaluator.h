#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class FollowsEvaluator : public Evaluator {
public:
	FollowsEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
