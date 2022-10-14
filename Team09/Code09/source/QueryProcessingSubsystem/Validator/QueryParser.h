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
	std::vector<Declaration> parseDeclaration();

	/**
	 * Checks if the given name is a synonym in the declaration list.
	 * @returns The declaration object with the given name.
	 * @throws SemanticError if the name is not declared.
	 */
	Declaration findDeclaration(std::string name);

	/**
	 * Creates a reference object from the given string.
	 * @returns A reference object.
	 */
	Reference getReference(std::string arg);

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

	Reference parseEntRef(std::vector<Declaration::DesignEntity> de);
	
	Reference parseStmtRef(std::vector<Declaration::DesignEntity> de);

	/**
	 * Parses select statement.
	 * @returns the target from the select statement.
	 */
	std::variant<Declaration, AttrReference> parseSelect();

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
	 * Determines whether the relation type is UsesS/ModifiesS or UsesP/ModifiesP
	 * @returns The correct relation type.
	 */
	Relation::Types getUsesModifiesType(Relation::Types type);

	/**
	 * Parses the such that clause.
	 * @returns the relation.
	 * @throws SemanticError if an argument is not valid.
	 */
	Relation suchThatClause();

	void parseSuchThat();

public:
	QueryParser(std::vector<std::string> tokens) : 
		query_tokens(tokens),
		index(0),
		current_token(getNextToken()), 
		declarations(std::vector<Declaration>()),
		target(std::variant<Declaration, AttrReference>()),
		suchThatCl(std::vector<Relation>()),
		patternCl(std::vector<Pattern>()),
		withCl(std::vector<With>()) {};

	~QueryParser() {};

	/**
	 * Parses the query tokens.
	 * @return A Query that has been parsed.
	 */
	Query* parse();
};
