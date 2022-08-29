#pragma once
#include <string>
#include <vector>
#include "RelationshipEvaluator.h"

class ParentEvaluator : public RelationshipEvaluator {
public:
	ParentEvaluator() {}; // Constructor

	std::vector<std::string> evaluate() override {
		return {};
	}
};
