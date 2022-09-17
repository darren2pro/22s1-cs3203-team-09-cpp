#pragma once
#include <string>
#include <vector>
#include "Relation.h"
#include "Pattern.h"
#include "Declaration.h"

class Query {
public:
	Relation relations;
	Pattern patterns;
	std::vector<Declaration> declarations;
	Declaration target;
	std::vector<std::string> results;


	Query() :
		relations(Relation()),
		patterns(Pattern()),
		declarations(std::vector<Declaration>()),
		target(Declaration()),
		results(std::vector<std::string>()) {};
};
