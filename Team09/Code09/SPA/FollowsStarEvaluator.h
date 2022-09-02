#pragma once
#include <string>
#include <vector>
#include "Evaluator.h"

class FollowsStarEvaluator : public Evaluator {
public:
	FollowsStarEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
