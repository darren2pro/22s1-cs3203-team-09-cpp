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
	Declaration target;					// (sprint 4) type Result	-->		Enum {Declaration, Boolean, Tuple}
	std::vector<std::string> results;	// (sprint 4) vector of Results? 


	Query() :
		relations(Relation()),
		patterns(Pattern()),
		declarations(std::vector<Declaration>()),
		target(Declaration()),
		results(std::vector<std::string>()) {};
};
