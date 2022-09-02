#pragma once
#include <string>
#include <regex>
#include <vector>
#include "Query.h"

/**
 * A QueryParser class to parse the query.
 */
class QueryParser {
private:
	/**
	 * A vector of tokens.
	 */
	std::vector<std::string> query_tokens;

	/**
	 * Index of current token.
	 */
	int index;

	/**
	 * A string that stores the current token.
	 */
	std::string current_token;

public:
	QueryParser(std::vector<std::string> tokens);

	~QueryParser();

	/**
	 * Returns the next token in query_tokens.
	 * @return A string of the next token.
	 */
	std::string getNextToken();

	/**
	 * Checks if the current token matches with the expected token.
	 * @throws QueryLParserException if the current token does not match the expected token.
	 */
	void match(std::string token);

	/**
	 * Checks if the current token matches with the expected regex.
	 * @throws QueryLParserException if the current token does not match the regex.
	 */
	void match(std::regex re);

	std::string declaration();

	std::string select();

	std::vector<std::string> patternClause();
	
	std::string suchThatClause();

	/**
	 * Parses the query tokens.
	 * @return A Query.
	 */
	Query parse();
};
