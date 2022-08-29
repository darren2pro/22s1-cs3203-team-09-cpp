#pragma once
#include <string>
#include <vector>
#include "RelationshipEvaluator.h"

class FollowsEvaluator : public RelationshipEvaluator {
public:
	FollowsEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
