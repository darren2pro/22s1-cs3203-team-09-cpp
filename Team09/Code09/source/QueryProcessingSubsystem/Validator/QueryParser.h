#pragma once
#include <string>
#include <regex>
#include <vector>
#include "../Query.h"
#include "../Relation.h"
#include "../Declaration.h"
#include "../Pattern.h"

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

	/**
	 * Query attributes.
	 */
	std::vector<Declaration> declarations;
	Declaration target;
	Relation suchThatCl;
	Pattern patternCl;

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
	 * @returns the current token.
	 * @throws SyntaxError if the current token does not match the expected token.
	 */
	std::string match(std::string token);

	/**
	 * Checks if the current token matches with the expected regex.
	 * @returns the current token.
	 * @throws SyntaxError if the current token does not match the regex.
	 */
	std::string match(std::regex re);

	/**
	 * Parses declaration.
	 * @returns the declarations.
	 * @throws SemanticError if there are duplcate synonyms.
	 */
	std::vector<Declaration> declaration();

	/**
	 * Checks if the given name is a synonym in the declaration list.
	 * @returns The declaration object with the given name.
	 * @throws SemanticError if the name is not declared.
	 */
	Declaration findDeclaration(std::string name);

	/**
	 * Checks that the arguent is a valid stmtRef.
	 * @throws SemanticError if arg is not valid.
	 */
	void validate_stmtRef(Relation::Types rel, std::string arg);
	
	/**
	 * Checks that the arguent is a valid entRef.
	 * @throws SemanticError if arg is not valid.
	 */
	void validate_entRef(std::string arg);

	/**
	 * Parses declaration.
	 * @returns the synonym in the select statement.
	 */
	Declaration select();

	/**
	 * Parses declaration.
	 * @returns the pattern clause.
	 * @throws SemanticError if an argument is not valid.
	 */
	Pattern patternClause();

	/**
	 * Parses the such that clause.
	 * @returns the relation.
	 * @throws SemanticError if an argument is not valid.
	 */
	Relation suchThatClause();

	/**
	 * Parses the query tokens.
	 * @return A Query.
	 */
	Query* parse();
};
