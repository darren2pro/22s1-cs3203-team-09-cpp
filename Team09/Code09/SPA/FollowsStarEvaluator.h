#pragma once
#include <string>
#include <vector>
#include "RelationshipEvaluator.h"

class FollowsStarEvaluator : public RelationshipEvaluator {
public:
	FollowsStarEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
