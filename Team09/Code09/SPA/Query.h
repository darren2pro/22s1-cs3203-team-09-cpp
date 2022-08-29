#pragma once
#include <string>
#include <vector>

class Query {
public:
	std::vector<std::string> relations;
	std::vector<std::string> patterns;
	std::vector<std::string> declarations;
	std::vector<std::string> tokens;

	Query();
};
