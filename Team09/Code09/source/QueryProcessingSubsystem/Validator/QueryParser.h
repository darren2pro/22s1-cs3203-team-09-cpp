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
	std::string target;
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
	 * @throws SyntaxError if the current token does not match the expected token.
	 */
	void match(std::string token);

	/**
	 * Checks if the current token matches with the expected regex.
	 * @throws SyntaxError if the current token does not match the regex.
	 */
	void match(std::regex re);

	/**
	 * Returns the Declaration::DesignEntity that is equivalent to the token string.
	 * @returns a Declaration::DesignEntity that is equivalent to the token string.
	 */
	Declaration::DesignEntity getDesignEntity(std::string token);

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
	 * @returns the variable in the select statement.
	 */
	std::string select();

	/**
	 * Parses declaration.
	 * @returns the pattern clause.
	 * @throws SemanticError if an argument is not valid.
	 */
	Pattern patternClause();

	/**
	 * Returns the Relation::Types that is equivalent to the token string.
	 * @returns a Realtion::Types that is equivalent to the token string.
	 */
	Relation::Types getType(std::string token);

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
