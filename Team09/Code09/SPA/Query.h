#pragma once
#include <string>
#include <vector>
#include "Relation.h"

class Query {
public:
	Relation relations;
	std::vector<std::string> patterns;
	std::vector<std::string> declarations;
	std::vector<std::string> tokens;
	std::vector<std::string> results;

	Query() :
		relations(Relation()), // Replace with new Relation after Parser class
		patterns(std::vector<std::string>()),
		declarations(std::vector<std::string>()),
		tokens(std::vector<std::string>()),
		results(std::vector<std::string>()) {};
};
