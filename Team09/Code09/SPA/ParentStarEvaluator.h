#pragma once
#include <string>
#include <vector>
#include "RelationshipEvaluator.h"

class ParentStarEvaluator : public RelationshipEvaluator {
public:
	ParentStarEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
