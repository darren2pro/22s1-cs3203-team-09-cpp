#pragma once
#include <string>
#include <regex>
#include <variant>
#include <vector>
#include "../Query.h"
#include "../Relation.h"
#include "../Declaration.h"
#include "../Pattern.h"
#include "../With.h"
#include "../AttrReference.h"

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
	std::variant<Declaration, AttrReference> target;
	std::vector<Relation> suchThatCl;
	std::vector<Pattern> patternCl;
	std::vector<With> withCl;

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
	 * Checks that ref is a valid stmtRef.
	 * @returns true if ref is a valid stmtRef, otherwise returns false.
	 */
	bool is_valid_stmtRef(Reference ref, std::vector<Declaration::DesignEntity> valid_types);
	
	/**
	* Checks that ref is a valid entRef.
	* @returns true if ref is a valid entRef, otherwise returns false.
	*/
	bool is_valid_entRef(Reference ref, std::vector<Declaration::DesignEntity> valid_types);

	/**
	 * Parses result clause.
	 * @returns the result clause from the select statement.
	 */
	std::variant<Declaration, AttrReference> select();

	AttrReference parseAttrRef();
	 
	/**
	 * Parses with clause.
	 * @returns the with clause.
	 */
	With withClause();

	void parseWith();

	/**
	 * Parses pattern clause.
	 * @returns the pattern clause.
	 * @throws SemanticError if an argument is not valid.
	 */
	Pattern patternClause();

	void parsePattern();

	/**
	 * Parses the such that clause.
	 * @returns the relation.
	 * @throws SemanticError if an argument is not valid.
	 */
	Relation suchThatClause();

	void parseSuchThat();

	/**
	 * Parses the query tokens.
	 * @return A Query.
	 */
	Query* parse();
};
