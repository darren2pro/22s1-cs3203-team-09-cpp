#pragma once
#include <string>
#include <vector>
#include <variant>
#include "Relation.h"
#include "Pattern.h"
#include "Declaration.h"
#include "With.h"
#include "AttrReference.h"

class Query {
public:
	std::vector<Relation> relations;
	std::vector<Pattern> patterns;
	std::vector<With> withs;
	std::vector<Declaration> declarations;
	std::variant<Declaration, AttrReference> target;					// (sprint 5) type Result for { Boolean, Tuple}
	std::vector<std::string> results;


	Query() :
		relations(std::vector<Relation>()),
		patterns(std::vector<Pattern>()),
		withs(std::vector<With>()),
		declarations(std::vector<Declaration>()),
		target(std::variant<Declaration, AttrReference>()),
		results(std::vector<std::string>()) {};

	bool operator==(const Query& q) const {
		return relations == q.relations && patterns == q.patterns && declarations == q.declarations && target == q.target && results == q.results && withs == q.withs;
	}
};
