#pragma once
#include <cctype>
#include <vector>
#include <string>

/**
 * A QueryLexer class to tokenize the query.
 */
class QueryLexer
{
private:
	/**
	 * The query string input.
	 */
	std::string query_string;

public:
	QueryLexer(std::string query);

	~QueryLexer();

	/**
	 * Tokenizes the query string.
	 * @return A vector of tokens.
	 * @throws SyntaxError if an unexpected character is encountered.
	 */
	std::vector<std::string> lex(); 

};
