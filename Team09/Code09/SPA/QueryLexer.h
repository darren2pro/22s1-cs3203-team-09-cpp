#pragma once
#include <cctype>
#include <vector>
#include <string>

class QueryLexer
{
public:
	std::string query_string;

	QueryLexer(std::string query);
	bool processNonAlphabetics();
	bool processAlphabetics();
	std::vector<std::string> lex(); // Returns the list of relevant strings

};